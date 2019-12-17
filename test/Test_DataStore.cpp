#include <gtest/gtest.h>

#include <chrono>
#include <ctime>
#include <iostream>
#include <memory>
#include <thread>

#include "../src/Datastore/RAMDataStore.h"
#include "../src/Types/Result.h"
#include "../src/Utils/ConfigHandler.h"

using namespace std;

TEST(DataStoreTest, OneisOne) {
  ASSERT_EQ(1, 1);
}

TEST(DataStoreTest, GetQueue_Empty) {
  RAMDataStore ds;

  // Queue with no element returns empty queues
  auto res = ds.getQueue(QueueType::Admin);
  ASSERT_EQ(checkAlternativeError(res), false);
  Queue q = get<Queue>(res);
  ASSERT_EQ(q.tracks.size(), 0);
  res = ds.getQueue(QueueType::Normal);
  ASSERT_EQ(checkAlternativeError(res), false);
  q = get<Queue>(res);
  ASSERT_EQ(q.tracks.size(), 0);
}

TEST(DataStoreTest, GetQueue_1Normal) {
  RAMDataStore ds;
  BaseTrack tr;
  tr.trackId = "trackId_example";
  tr.title = "title_example";
  tr.album = "album_example";
  tr.artist = "artist_example";
  tr.durationMs = 100;
  tr.iconUri = "iconUri_example";
  tr.addedBy = "addedBy_example";

  ds.addTrack(tr, QueueType::Normal);

  auto res = ds.getQueue(QueueType::Admin);
  ASSERT_EQ(checkAlternativeError(res), false);
  Queue q = get<Queue>(res);
  ASSERT_EQ(q.tracks.size(), 0);
  res = ds.getQueue(QueueType::Normal);
  ASSERT_EQ(checkAlternativeError(res), false);
  q = get<Queue>(res);
  ASSERT_EQ(q.tracks.size(), 1);
  ASSERT_EQ(q.tracks[0].trackId, tr.trackId);
  ASSERT_EQ(q.tracks[0].title, tr.title);
  ASSERT_EQ(q.tracks[0].album, tr.album);
  ASSERT_EQ(q.tracks[0].artist, tr.artist);
  ASSERT_EQ(q.tracks[0].durationMs, tr.durationMs);
  ASSERT_EQ(q.tracks[0].iconUri, tr.iconUri);
  ASSERT_EQ(q.tracks[0].addedBy, tr.addedBy);
}

TEST(DataStoreTest, GetQueue_1Admin) {
  RAMDataStore ds;
  BaseTrack tr;
  tr.trackId = "trackId_example";
  tr.title = "title_example";
  tr.album = "album_example";
  tr.artist = "artist_example";
  tr.durationMs = 100;
  tr.iconUri = "iconUri_example";
  tr.addedBy = "addedBy_example";

  ds.addTrack(tr, QueueType::Admin);

  auto res = ds.getQueue(QueueType::Normal);
  ASSERT_EQ(checkAlternativeError(res), false);
  Queue q = get<Queue>(res);
  ASSERT_EQ(q.tracks.size(), 0);
  res = ds.getQueue(QueueType::Admin);
  ASSERT_EQ(checkAlternativeError(res), false);
  q = get<Queue>(res);
  ASSERT_EQ(q.tracks.size(), 1);
  ASSERT_EQ(q.tracks[0].trackId, tr.trackId);
  ASSERT_EQ(q.tracks[0].title, tr.title);
  ASSERT_EQ(q.tracks[0].album, tr.album);
  ASSERT_EQ(q.tracks[0].artist, tr.artist);
  ASSERT_EQ(q.tracks[0].durationMs, tr.durationMs);
  ASSERT_EQ(q.tracks[0].iconUri, tr.iconUri);
  ASSERT_EQ(q.tracks[0].addedBy, tr.addedBy);
}

TEST(DataStoreTest, UserAddHas) {
  RAMDataStore ds;
  User usr1;
  usr1.SessionID = "usr1_sessionID";
  usr1.isAdmin = false;
  usr1.ExpirationDate = 0xFFFFFFFFFF;
  usr1.Name = "Hans";
  User usr2;
  usr2.SessionID = "usr2_sessionID";
  usr2.isAdmin = true;
  usr2.ExpirationDate = 0xFFFFFFFFFFA;
  usr2.Name = "admin";

  // user not present
  ASSERT_EQ(ds.hasUser(usr1.SessionID), false);

  // add 2 users and check for both of them
  ds.addUser(usr1);
  ds.addUser(usr2);
  ASSERT_EQ(ds.hasUser(usr1.SessionID), true);
  ASSERT_EQ(ds.hasUser(usr2.SessionID), true);

  // remove 1 user and check for both
  ds.removeUser(usr1.SessionID);
  ASSERT_EQ(ds.hasUser(usr1.SessionID), false);
  ASSERT_EQ(ds.hasUser(usr2.SessionID), true);

  // remove other user and check
  ds.removeUser(usr2.SessionID);
  ASSERT_EQ(ds.hasUser(usr1.SessionID), false);
  ASSERT_EQ(ds.hasUser(usr2.SessionID), false);
}

TEST(DataStoreTest, UserTimeout) {
  RAMDataStore ds;
  User usr1;
  usr1.SessionID = "usr1_sessionID";
  usr1.isAdmin = false;
  usr1.ExpirationDate = time(nullptr) + 2;
  usr1.Name = "Hans";
  User usr2;
  usr2.SessionID = "usr2_sessionID";
  usr2.isAdmin = true;
  usr2.ExpirationDate = time(nullptr) + 4;
  usr2.Name = "admin";

  // user not present
  ASSERT_EQ(ds.hasUser(usr1.SessionID), false);

  // add 2 users and check for both of them
  ds.addUser(usr1);
  ds.addUser(usr2);
  ASSERT_EQ(ds.hasUser(usr1.SessionID), true);
  ASSERT_EQ(ds.hasUser(usr2.SessionID), true);

  // sleep 3 seconds, first user should timeout then
  this_thread::sleep_for(chrono::seconds(3));
  auto res = ds.isSessionExpired(usr1.SessionID);
  ASSERT_EQ(checkAlternativeError(res), true);
  ASSERT_EQ(ds.hasUser(usr1.SessionID), true);
  ASSERT_EQ(ds.hasUser(usr2.SessionID), true);

  // sleep another 3 seconds, second user should timeout then
  this_thread::sleep_for(chrono::seconds(3));
  res = ds.isSessionExpired(usr2.SessionID);
  ASSERT_EQ(checkAlternativeError(res), true);
  ASSERT_EQ(ds.hasUser(usr1.SessionID), true);
  ASSERT_EQ(ds.hasUser(usr2.SessionID), true);
}

TEST(DataStoreTest, votetest) {
  RAMDataStore ds;
  BaseTrack tr1;
  tr1.trackId = "song1";
  tr1.title = "title_example";
  tr1.album = "album_example";
  tr1.artist = "artist_example";
  tr1.durationMs = 100;
  tr1.iconUri = "iconUri_example";
  tr1.addedBy = "addedBy_example";
  BaseTrack tr2;
  tr2.trackId = "song2";
  tr2.title = "tr2_title";
  tr2.album = "tr2_album";
  tr2.artist = "tr2_artist";
  tr2.durationMs = 10230;
  tr2.iconUri = "tr2_iconUri";
  tr2.addedBy = "tr2_addedBy";
  BaseTrack tr3;
  tr3.trackId = "song3";
  tr3.title = "tr2_title";
  tr3.album = "tr2_album";
  tr3.artist = "tr2_artist";
  tr3.durationMs = 10230;
  tr3.iconUri = "tr2_iconUri";
  tr3.addedBy = "tr2_addedBy";
  BaseTrack tr4;
  tr4.trackId = "song4";
  tr4.title = "tr2_title";
  tr4.album = "tr2_album";
  tr4.artist = "tr2_artist";
  tr4.durationMs = 10230;
  tr4.iconUri = "tr2_iconUri";
  tr4.addedBy = "tr2_addedBy";

  User usr1;
  usr1.SessionID = "usr1_sessionID";
  usr1.isAdmin = false;
  usr1.ExpirationDate = time(nullptr) + 2;
  usr1.Name = "Hans";
  User usr2;
  usr2.SessionID = "usr2_sessionID";
  usr2.isAdmin = true;
  usr2.ExpirationDate = time(nullptr) + 4;
  usr2.Name = "admin";

  auto add_res = ds.addTrack(tr1, QueueType::Admin);
  ASSERT_EQ(checkOptionalError(add_res), false);
  add_res = ds.addTrack(tr2, QueueType::Admin);
  ASSERT_EQ(checkOptionalError(add_res), false);

  add_res = ds.addTrack(tr1, QueueType::Normal);
  ASSERT_EQ(checkOptionalError(add_res), true);
  add_res = ds.addTrack(tr2, QueueType::Normal);
  ASSERT_EQ(checkOptionalError(add_res), true);
  add_res = ds.addTrack(tr3, QueueType::Normal);
  ASSERT_EQ(checkOptionalError(add_res), false);
  add_res = ds.addTrack(tr4, QueueType::Normal);
  ASSERT_EQ(checkOptionalError(add_res), false);

  auto res = ds.getQueue(QueueType::Admin);
  ASSERT_EQ(checkAlternativeError(res), false);
  Queue q = get<Queue>(res);
  ASSERT_EQ(q.tracks.size(), 2);
  ASSERT_EQ(q.tracks[0].trackId, tr1.trackId);
  ASSERT_EQ(q.tracks[0].title, tr1.title);
  ASSERT_EQ(q.tracks[0].album, tr1.album);
  ASSERT_EQ(q.tracks[0].artist, tr1.artist);
  ASSERT_EQ(q.tracks[0].durationMs, tr1.durationMs);
  ASSERT_EQ(q.tracks[0].iconUri, tr1.iconUri);
  ASSERT_EQ(q.tracks[0].addedBy, tr1.addedBy);
  ASSERT_EQ(q.tracks[1].trackId, tr2.trackId);
  ASSERT_EQ(q.tracks[1].title, tr2.title);
  ASSERT_EQ(q.tracks[1].album, tr2.album);
  ASSERT_EQ(q.tracks[1].artist, tr2.artist);
  ASSERT_EQ(q.tracks[1].durationMs, tr2.durationMs);
  ASSERT_EQ(q.tracks[1].iconUri, tr2.iconUri);
  ASSERT_EQ(q.tracks[1].addedBy, tr2.addedBy);

  res = ds.getQueue(QueueType::Normal);
  ASSERT_EQ(checkAlternativeError(res), false);
  q = get<Queue>(res);
  ASSERT_EQ(q.tracks.size(), 2);
  ASSERT_EQ(q.tracks[0].trackId, tr3.trackId);
  ASSERT_EQ(q.tracks[0].title, tr3.title);
  ASSERT_EQ(q.tracks[0].album, tr3.album);
  ASSERT_EQ(q.tracks[0].artist, tr3.artist);
  ASSERT_EQ(q.tracks[0].durationMs, tr3.durationMs);
  ASSERT_EQ(q.tracks[0].iconUri, tr3.iconUri);
  ASSERT_EQ(q.tracks[0].addedBy, tr3.addedBy);
  ASSERT_EQ(q.tracks[1].trackId, tr4.trackId);
  ASSERT_EQ(q.tracks[1].title, tr4.title);
  ASSERT_EQ(q.tracks[1].album, tr4.album);
  ASSERT_EQ(q.tracks[1].artist, tr4.artist);
  ASSERT_EQ(q.tracks[1].durationMs, tr4.durationMs);
  ASSERT_EQ(q.tracks[1].iconUri, tr4.iconUri);
  ASSERT_EQ(q.tracks[1].addedBy, tr4.addedBy);

  // add users and check for them
  ds.addUser(usr1);
  ds.addUser(usr2);
  ASSERT_EQ(ds.hasUser(usr1.SessionID), true);
  ASSERT_EQ(ds.hasUser(usr2.SessionID), true);

  ds.voteTrack(usr1.SessionID, tr3.trackId, true);

  res = ds.getQueue(QueueType::Admin);
  ASSERT_EQ(checkAlternativeError(res), false);
  q = get<Queue>(res);
  ASSERT_EQ(q.tracks.size(), 2);
  ASSERT_EQ(q.tracks[0].trackId, tr1.trackId);
  ASSERT_EQ(q.tracks[0].title, tr1.title);
  ASSERT_EQ(q.tracks[0].album, tr1.album);
  ASSERT_EQ(q.tracks[0].artist, tr1.artist);
  ASSERT_EQ(q.tracks[0].durationMs, tr1.durationMs);
  ASSERT_EQ(q.tracks[0].iconUri, tr1.iconUri);
  ASSERT_EQ(q.tracks[0].addedBy, tr1.addedBy);
  ASSERT_EQ(q.tracks[0].votes, 0);
  ASSERT_EQ(q.tracks[1].trackId, tr2.trackId);
  ASSERT_EQ(q.tracks[1].title, tr2.title);
  ASSERT_EQ(q.tracks[1].album, tr2.album);
  ASSERT_EQ(q.tracks[1].artist, tr2.artist);
  ASSERT_EQ(q.tracks[1].durationMs, tr2.durationMs);
  ASSERT_EQ(q.tracks[1].iconUri, tr2.iconUri);
  ASSERT_EQ(q.tracks[1].addedBy, tr2.addedBy);
  ASSERT_EQ(q.tracks[1].votes, 0);

  res = ds.getQueue(QueueType::Normal);
  ASSERT_EQ(checkAlternativeError(res), false);
  q = get<Queue>(res);
  ASSERT_EQ(q.tracks.size(), 2);
  ASSERT_EQ(q.tracks[0].trackId, tr3.trackId);
  ASSERT_EQ(q.tracks[0].title, tr3.title);
  ASSERT_EQ(q.tracks[0].album, tr3.album);
  ASSERT_EQ(q.tracks[0].artist, tr3.artist);
  ASSERT_EQ(q.tracks[0].durationMs, tr3.durationMs);
  ASSERT_EQ(q.tracks[0].iconUri, tr3.iconUri);
  ASSERT_EQ(q.tracks[0].addedBy, tr3.addedBy);
  ASSERT_EQ(q.tracks[0].votes, 1);
  ASSERT_EQ(q.tracks[1].trackId, tr4.trackId);
  ASSERT_EQ(q.tracks[1].title, tr4.title);
  ASSERT_EQ(q.tracks[1].album, tr4.album);
  ASSERT_EQ(q.tracks[1].artist, tr4.artist);
  ASSERT_EQ(q.tracks[1].durationMs, tr4.durationMs);
  ASSERT_EQ(q.tracks[1].iconUri, tr4.iconUri);
  ASSERT_EQ(q.tracks[1].addedBy, tr4.addedBy);
  ASSERT_EQ(q.tracks[1].votes, 0);

  // remove upvote, upvote other track
  ds.voteTrack(usr1.SessionID, tr3.trackId, false);
  ds.voteTrack(usr1.SessionID, tr4.trackId, true);

  res = ds.getQueue(QueueType::Admin);
  ASSERT_EQ(checkAlternativeError(res), false);
  q = get<Queue>(res);
  ASSERT_EQ(q.tracks.size(), 2);
  // different order than usual because the queue got resorted in normal queue,
  // same order as before in Admin q because that one doesnt get resorted
  ASSERT_EQ(q.tracks[0].trackId, tr1.trackId);
  ASSERT_EQ(q.tracks[0].title, tr1.title);
  ASSERT_EQ(q.tracks[0].album, tr1.album);
  ASSERT_EQ(q.tracks[0].artist, tr1.artist);
  ASSERT_EQ(q.tracks[0].durationMs, tr1.durationMs);
  ASSERT_EQ(q.tracks[0].iconUri, tr1.iconUri);
  ASSERT_EQ(q.tracks[0].addedBy, tr1.addedBy);
  ASSERT_EQ(q.tracks[0].votes, 0);
  ASSERT_EQ(q.tracks[1].trackId, tr2.trackId);
  ASSERT_EQ(q.tracks[1].title, tr2.title);
  ASSERT_EQ(q.tracks[1].album, tr2.album);
  ASSERT_EQ(q.tracks[1].artist, tr2.artist);
  ASSERT_EQ(q.tracks[1].durationMs, tr2.durationMs);
  ASSERT_EQ(q.tracks[1].iconUri, tr2.iconUri);
  ASSERT_EQ(q.tracks[1].addedBy, tr2.addedBy);
  ASSERT_EQ(q.tracks[1].votes, 0);

  res = ds.getQueue(QueueType::Normal);
  ASSERT_EQ(checkAlternativeError(res), false);
  q = get<Queue>(res);
  ASSERT_EQ(q.tracks.size(), 2);
  ASSERT_EQ(q.tracks[0].trackId, tr4.trackId);
  ASSERT_EQ(q.tracks[0].title, tr4.title);
  ASSERT_EQ(q.tracks[0].album, tr4.album);
  ASSERT_EQ(q.tracks[0].artist, tr4.artist);
  ;
  ASSERT_EQ(q.tracks[0].durationMs, tr4.durationMs);
  ASSERT_EQ(q.tracks[0].iconUri, tr4.iconUri);
  ASSERT_EQ(q.tracks[0].addedBy, tr4.addedBy);
  ASSERT_EQ(q.tracks[0].votes, 1);
  ASSERT_EQ(q.tracks[1].title, tr3.title);
  ASSERT_EQ(q.tracks[1].album, tr3.album);
  ASSERT_EQ(q.tracks[1].artist, tr3.artist);
  ASSERT_EQ(q.tracks[1].durationMs, tr3.durationMs);
  ASSERT_EQ(q.tracks[1].iconUri, tr3.iconUri);
  ASSERT_EQ(q.tracks[1].addedBy, tr3.addedBy);
  ASSERT_EQ(q.tracks[1].votes, 0);

  // test nextTrack
  // track 1 from adminqueue
  ds.nextTrack();
  auto restr = ds.getPlayingTrack();
  ASSERT_EQ(checkAlternativeError(restr), false);
  QueuedTrack tr = get<optional<QueuedTrack>>(restr).value();
  ASSERT_EQ(tr.trackId, tr1.trackId);
  ASSERT_EQ(tr.title, tr1.title);
  ASSERT_EQ(tr.album, tr1.album);
  ASSERT_EQ(tr.artist, tr1.artist);
  ASSERT_EQ(tr.durationMs, tr1.durationMs);
  ASSERT_EQ(tr.iconUri, tr1.iconUri);
  ASSERT_EQ(tr.addedBy, tr1.addedBy);

  // track 2 from adminqueue
  ds.nextTrack();
  restr = ds.getPlayingTrack();
  ASSERT_EQ(checkAlternativeError(restr), false);
  tr = get<optional<QueuedTrack>>(restr).value();
  ASSERT_EQ(tr.trackId, tr2.trackId);
  ASSERT_EQ(tr.title, tr2.title);
  ASSERT_EQ(tr.album, tr2.album);
  ASSERT_EQ(tr.artist, tr2.artist);
  ASSERT_EQ(tr.durationMs, tr2.durationMs);
  ASSERT_EQ(tr.iconUri, tr2.iconUri);
  ASSERT_EQ(tr.addedBy, tr2.addedBy);

  // track 4 Because it got upvoted
  ds.nextTrack();
  restr = ds.getPlayingTrack();
  ASSERT_EQ(checkAlternativeError(restr), false);
  tr = get<optional<QueuedTrack>>(restr).value();
  ASSERT_EQ(tr.trackId, tr4.trackId);
  ASSERT_EQ(tr.title, tr4.title);
  ASSERT_EQ(tr.album, tr4.album);
  ASSERT_EQ(tr.artist, tr4.artist);
  ASSERT_EQ(tr.durationMs, tr4.durationMs);
  ASSERT_EQ(tr.iconUri, tr4.iconUri);
  ASSERT_EQ(tr.addedBy, tr4.addedBy);

  // track 3 from normal queue
  ds.nextTrack();
  restr = ds.getPlayingTrack();
  ASSERT_EQ(checkAlternativeError(restr), false);
  tr = get<optional<QueuedTrack>>(restr).value();
  ASSERT_EQ(tr.trackId, tr3.trackId);
  ASSERT_EQ(tr.title, tr3.title);
  ASSERT_EQ(tr.album, tr3.album);
  ASSERT_EQ(tr.artist, tr3.artist);
  ASSERT_EQ(tr.durationMs, tr3.durationMs);
  ASSERT_EQ(tr.iconUri, tr3.iconUri);
  ASSERT_EQ(tr.addedBy, tr3.addedBy);

  // Queues empty
  ds.nextTrack();
  restr = ds.getPlayingTrack();
  ASSERT_EQ(checkAlternativeError(restr), false);
}
