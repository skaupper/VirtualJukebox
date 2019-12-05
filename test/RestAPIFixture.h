/*****************************************************************************/
/**
 * @file    RestAPIFixture.h
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Definition of a test fixture for the RestAPI tests
 */
/*****************************************************************************/

#ifndef _REST_API_FIXTURE_H_
#define _REST_API_FIXTURE_H_

#include <gtest/gtest.h>


class RestAPIFixture : public ::testing::Test {
public:

protected:
    void SetUp() override;
};

#endif /* _REST_API_FIXTURE_H_ */
