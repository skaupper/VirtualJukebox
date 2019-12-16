/*****************************************************************************/
/**
 * @file    MusicBackend.h
 * @author  Stefan Jahn <stefan.jahn332@gmail.com>
 * @brief   Interface for MusicBackend
 */
/*****************************************************************************/

#ifndef _MUSICBACKEND_H_
#define _MUSICBACKEND_H_

#include <memory>
#include <vector>

#include "Types/GlobalTypes.h"
#include "Types/Queue.h"
#include "Types/Result.h"

/**
 * @brief Provides an abstracted interface to a MusicBackend s.
 */
class MusicBackend {
 public:
  virtual ~MusicBackend() {
  }

  /**
   * @brief Initalizes the backend.
   */
  virtual TResultOpt initBackend() = 0;

  /**
   * @brief Queries the backend for all tracks given a pattern.
   * @param pattern Search patten (wildcard support depends on the backend).
   * @param num Maximum number of returned tracks (maximum is 50).
   * @return On success returns a vector of BaseTrack, otherwise Error.
   */
  virtual TResult<std::vector<BaseTrack>> queryTracks(
      std::string const &pattern, size_t const num) = 0;

  /**
   * @brief Set the given track as playback (means the track gets played).
   * @param track track to play
   * @return Returns an Error on failure.
   */
  virtual TResultOpt setPlayback(BaseTrack const &track) = 0;

  /**
   * @brief Returns the current playback of the active device as a playback
   * track.
   * @return Current playback track on success (or nothing if no track is
   * currently playing), otherwise Error.
   */
  virtual TResult<std::optional<PlaybackTrack>> getCurrentPlayback() = 0;

  /**
   * @brief Pauses the current playback.
   * @return Returns an Error on failure.
   */
  virtual TResultOpt pause() = 0;

  /**
   * @brief Resumes the current playback.
   * @return Returns an Error on failure.
   */
  virtual TResultOpt play() = 0;

  /**
   * @brief Returns the volume of the actual device in percent.
   * @return Volume in percent on success, otherwise Error.
   */
  virtual TResult<size_t> getVolume() = 0;

  /**
   * @brief Sets a new volume.
   * @details If a device is given in the ini file, this device gets selected.
   * If no such device is given, the actual active device is used.
   * @param percent Volume in percent.
   * @return Returns an Error on failure.
   */
  virtual TResultOpt setVolume(size_t const percent) = 0;

  /**
   * @brief Creates a new BaseTrack instance with the given trackID.
   * @param trackID Unique track ID (returned for example by queryTracks).
   * @return New BaseTrack object on success, Error otherwise
   */
  virtual TResult<BaseTrack> createBaseTrack(TTrackID const &trackID) = 0;
};

#endif /* _MUSICBACKEND_H_ */
