#include <gtest.h>
#include <gmock.h>

#include "AI/A_star.hpp"

using namespace testing;

class TestableAStar : public Test
{
protected:
    AStar::Tile go = AStar::Tile{true};
    AStar::Tile no = AStar::Tile{false};
};

class InvalidParams : public TestableAStar
{
protected:
    AStar::TileMap map = { {go} };
};

TEST_F(InvalidParams, StartOrEndOutsideMapIndices)
{
    AStar::Location start = {0, 0};
    AStar::Location end   = {0, 0};

    // start not in map
    start = {-1, 0};
    EXPECT_FALSE(AStar::run(start, end, map).has_path);

    start = {1, 0};
    EXPECT_FALSE(AStar::run(start, end, map).has_path);

    start = {0, -1};
    EXPECT_FALSE(AStar::run(start, end, map).has_path);

    start = {0, 1};
    EXPECT_FALSE(AStar::run(start, end, map).has_path);

    // end not in map
    end = {-1, 0};
    EXPECT_FALSE(AStar::run(start, end, map).has_path);

    end = {1, 0};
    EXPECT_FALSE(AStar::run(start, end, map).has_path);

    end = {0, -1};
    EXPECT_FALSE(AStar::run(start, end, map).has_path);

    end = {0, 1};
    EXPECT_FALSE(AStar::run(start, end, map).has_path);
}

class OneDimensionalMapInY : public TestableAStar
{
protected:
    AStar::TileMap map = {
        {no, go, go, go}
    };
};

TEST_F(OneDimensionalMapInY, UnblockedFromStartToFinish_BecauseStartIsEnd)
{
    AStar::Location start = {0, 0};
    AStar::Location end   = {0, 0};
    ASSERT_EQ(start, end);
    EXPECT_EQ(map.at(start.y).at(start.x), no);

    auto path = AStar::run(start, end, map);

    EXPECT_TRUE(path.has_path);

    ASSERT_EQ(1, path.path.size());
    EXPECT_EQ(AStar::Location(start), path.path[0]);
}

TEST_F(OneDimensionalMapInY, UnblockedFromStartToFinish_BecausePathIsPassable)
{
    AStar::Location start = {0, 0};
    AStar::Location end   = {0, 3};
    ASSERT_EQ(map.at(start.y).at(start.x), no);
    ASSERT_EQ(map.at(end.y).at(end.x), go);

    auto path = AStar::run(start, end, map);

    EXPECT_TRUE(path.has_path);

    ASSERT_EQ(4, path.path.size());
    EXPECT_EQ(AStar::Location(start),  path.path[0]);
    EXPECT_EQ(AStar::Location({0, 1}), path.path[1]);
    EXPECT_EQ(AStar::Location({0, 2}), path.path[2]);
    EXPECT_EQ(AStar::Location(end),    path.path[3]);
}

class OneDimensionalMapInX : public TestableAStar
{
protected:
    AStar::TileMap map = {
        {no},
        {go},
        {go},
        {go},
    };
};

TEST_F(OneDimensionalMapInX, UnblockedFromStartToFinish_BecauseStartIsEnd)
{
    AStar::Location start = {0, 0};
    AStar::Location end   = {0, 0};
    ASSERT_EQ(start, end);
    EXPECT_EQ(map.at(start.y).at(start.x), no);

    auto path = AStar::run(start, end, map);

    EXPECT_TRUE(path.has_path);

    ASSERT_EQ(1, path.path.size());
    EXPECT_EQ(AStar::Location(start), path.path[0]);
}

TEST_F(OneDimensionalMapInX, UnblockedFromStartToFinish_BecausePathIsPassable)
{
    AStar::Location start = {0, 0};
    AStar::Location end   = {3, 0};
    ASSERT_EQ(map.at(start.y).at(start.x), no);
    ASSERT_EQ(map.at(end.y).at(end.x), go);

    auto path = AStar::run(start, end, map);

    EXPECT_TRUE(path.has_path);

    ASSERT_EQ(4, path.path.size());
    EXPECT_EQ(AStar::Location(start),  path.path[0]);
    EXPECT_EQ(AStar::Location({1, 0}), path.path[1]);
    EXPECT_EQ(AStar::Location({2, 0}), path.path[2]);
    EXPECT_EQ(AStar::Location(end),    path.path[3]);
}
