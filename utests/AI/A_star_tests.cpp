#include <gtest.h>
#include <gmock.h>

#include "AI/A_star.hpp"

using namespace testing;

class TestableAStar : public Test
{
protected:
    AStar::Tile st = AStar::Tile{true};
    AStar::Tile nd = AStar::Tile{true};

    AStar::Tile go = AStar::Tile{true};
    AStar::Tile no = AStar::Tile{false};
};

class InvalidParams : public TestableAStar
{
protected:
    sf::Vector2i dimensions = {1, 1};

    AStar::TileMap map = { {st} };
};

TEST_F(InvalidParams, StartOrEndOutsideMapIndices)
{
    sf::Vector2i start = {0, 0};
    sf::Vector2i end   = {0, 0};

    // start not in map
    start = {-1, 0};
    EXPECT_FALSE(AStar::run(start, end, dimensions, map).has_path);

    start = {1, 0};
    EXPECT_FALSE(AStar::run(start, end, dimensions, map).has_path);

    start = {0, -1};
    EXPECT_FALSE(AStar::run(start, end, dimensions, map).has_path);

    start = {0, 1};
    EXPECT_FALSE(AStar::run(start, end, dimensions, map).has_path);

    // end not in map
    end = {-1, 0};
    EXPECT_FALSE(AStar::run(start, end, dimensions, map).has_path);

    end = {1, 0};
    EXPECT_FALSE(AStar::run(start, end, dimensions, map).has_path);

    end = {0, -1};
    EXPECT_FALSE(AStar::run(start, end, dimensions, map).has_path);

    end = {0, 1};
    EXPECT_FALSE(AStar::run(start, end, dimensions, map).has_path);
}

class OneDimensionalMapInY : public TestableAStar
{
protected:
    sf::Vector2i dimensions = {1, 1};
    AStar::TileMap map = {
        {st, go, go, nd}
    };
};

TEST_F(OneDimensionalMapInY, UnblockedFromStartToFinish_BecauseStartIsEnd)
{
    sf::Vector2i start = {0, 0};
    sf::Vector2i end   = {0, 0};
    ASSERT_EQ(start, end);
    EXPECT_EQ(map.at(start.y).at(start.x), st);

    auto path = AStar::run(start, end, dimensions, map);

    EXPECT_TRUE(path.has_path);

    ASSERT_EQ(1, path.path.size());
    EXPECT_EQ(sf::Vector2i(start), path.path[0]);
}

TEST_F(OneDimensionalMapInY, UnblockedFromStartToFinish_BecausePathIsPassable)
{
    sf::Vector2i start = {0, 0};
    sf::Vector2i end   = {3, 0};
    ASSERT_EQ(map.at(start.y).at(start.x), st);
    ASSERT_EQ(map.at(end.y).at(end.x), nd);

    auto path = AStar::run(start, end, dimensions, map);

    EXPECT_TRUE(path.has_path);

    ASSERT_EQ(4, path.path.size());
    EXPECT_EQ(sf::Vector2i(start),  path.path[0]);
    EXPECT_EQ(sf::Vector2i({1, 0}), path.path[1]);
    EXPECT_EQ(sf::Vector2i({2, 0}), path.path[2]);
    EXPECT_EQ(sf::Vector2i(end),    path.path[3]);
}

class OneDimensionalMapInX : public TestableAStar
{
protected:
    sf::Vector2i dimensions = {1, 1};

    AStar::TileMap map = {
        {st},
        {go},
        {go},
        {nd},
    };
};

TEST_F(OneDimensionalMapInX, UnblockedFromStartToFinish_BecauseStartIsEnd)
{
    sf::Vector2i start = {0, 0};
    sf::Vector2i end   = {0, 0};
    ASSERT_EQ(start, end);
    EXPECT_EQ(map.at(start.y).at(start.x), st);

    auto path = AStar::run(start, end, dimensions, map);

    EXPECT_TRUE(path.has_path);

    ASSERT_EQ(1, path.path.size());
    EXPECT_EQ(sf::Vector2i(start), path.path[0]);
}

TEST_F(OneDimensionalMapInX, UnblockedFromStartToFinish_BecausePathIsPassable)
{
    sf::Vector2i start = {0, 0};
    sf::Vector2i end   = {0, 3};
    ASSERT_EQ(map.at(start.y).at(start.x), st);
    ASSERT_EQ(map.at(end.y).at(end.x), nd);

    auto path = AStar::run(start, end, dimensions, map);

    EXPECT_TRUE(path.has_path);

    ASSERT_EQ(4, path.path.size());
    EXPECT_EQ(sf::Vector2i(start),  path.path[0]);
    EXPECT_EQ(sf::Vector2i({0, 1}), path.path[1]);
    EXPECT_EQ(sf::Vector2i({0, 2}), path.path[2]);
    EXPECT_EQ(sf::Vector2i(end),    path.path[3]);
}

class NarrowPassages : public TestableAStar
{
protected:
    sf::Vector2i dimensions = {1, 3};

    AStar::TileMap passable = {
        {no, no, no, no, no},
        {st, go, go, go, nd},
        {st, go, go, go, nd},
        {st, go, go, go, nd},
        {no, no, no, no, no},

    };

    AStar::TileMap impassable = {
        {no, no, no, no, no},
        {st, go, go, go, nd},
        {st, go, go, go, nd},
        {st, go, no, go, nd},
        {no, no, no, no, no},

    };
};

TEST_F(NarrowPassages, EntityIsSmallerThanGapWidth_HasPath)
{
    sf::Vector2i start = {0, 1};
    sf::Vector2i end   = {4, 1};
    ASSERT_EQ(passable.at(start.y).at(start.x), st);
    ASSERT_EQ(passable.at(end.y).at(end.x), nd);

    auto path = AStar::run(start, end, dimensions, passable);

    EXPECT_TRUE(path.has_path);

    ASSERT_EQ(5, path.path.size());
    EXPECT_EQ(sf::Vector2i(start),  path.path[0]);
    EXPECT_EQ(sf::Vector2i({1, 1}), path.path[1]);
    EXPECT_EQ(sf::Vector2i({2, 1}), path.path[2]);
    EXPECT_EQ(sf::Vector2i({3, 1}), path.path[3]);
    EXPECT_EQ(sf::Vector2i(end),    path.path[4]);
}

TEST_F(NarrowPassages, EntityIsTooWideToFitThroughNarrowGap_HasNoPath)
{
    sf::Vector2i start = {0, 1};
    sf::Vector2i end   = {4, 1};
    ASSERT_EQ(impassable.at(start.y).at(start.x), st);
    ASSERT_EQ(impassable.at(end.y).at(end.x), nd);

    auto path = AStar::run(start, end, dimensions, impassable);

    EXPECT_FALSE(path.has_path);
}

class VerticalObstables : public TestableAStar
{
protected:
    sf::Vector2i dimensions = {1, 2};

    AStar::TileMap map = {
   /*    0   1   2   3   4   5   6  */
   /*0*/{go, go, go, no, go, go, go},
   /*1*/{go, go, go, no, go, go, go},
   /*2*/{go, no, go, no, go, no, go},
   /*3*/{st, no, go, go, go, no, nd},
   /*4*/{st, no, go, go, go, no, nd},
    };
};

TEST_F(VerticalObstables, NavigatesAroundThem)
{
    sf::Vector2i start = {0, 3};
    sf::Vector2i end   = {6, 3};
    ASSERT_EQ(map.at(start.y).at(start.x), st);
    ASSERT_EQ(map.at(end.y).at(end.x), nd);

    auto path = AStar::run(start, end, dimensions, map);

    EXPECT_TRUE(path.has_path);

    ASSERT_EQ(19, path.path.size());
    EXPECT_EQ(sf::Vector2i(start),  path.path[0]);
    EXPECT_EQ(sf::Vector2i({0, 2}), path.path[1]);
    EXPECT_EQ(sf::Vector2i({0, 1}), path.path[2]);
    EXPECT_EQ(sf::Vector2i({0, 0}), path.path[3]);
    EXPECT_EQ(sf::Vector2i({1, 0}), path.path[4]);
    EXPECT_EQ(sf::Vector2i({2, 0}), path.path[5]);
    EXPECT_EQ(sf::Vector2i({2, 1}), path.path[6]);
    EXPECT_EQ(sf::Vector2i({2, 2}), path.path[7]);
    EXPECT_EQ(sf::Vector2i({2, 3}), path.path[8]);
    EXPECT_EQ(sf::Vector2i({3, 3}), path.path[9]);
    EXPECT_EQ(sf::Vector2i({4, 3}), path.path[10]);
    EXPECT_EQ(sf::Vector2i({4, 2}), path.path[11]);
    EXPECT_EQ(sf::Vector2i({4, 1}), path.path[12]);
    EXPECT_EQ(sf::Vector2i({4, 0}), path.path[13]);
    EXPECT_EQ(sf::Vector2i({5, 0}), path.path[14]);
    EXPECT_EQ(sf::Vector2i({6, 0}), path.path[15]);
    EXPECT_EQ(sf::Vector2i({6, 1}), path.path[16]);
    EXPECT_EQ(sf::Vector2i({6, 2}), path.path[17]);
    EXPECT_EQ(sf::Vector2i(end),    path.path[18]);
}

class HorizontalObstables : public TestableAStar
{
protected:
    sf::Vector2i dimensions = {2, 1};

    AStar::TileMap map = {
   /*    0   1   2   3   4  */
   /*0*/{nd, nd, go, go, go},
   /*1*/{no, no, no, go, go},
   /*2*/{go, go, go, go, go},
   /*3*/{go, go, no, no, no},
   /*4*/{go, go, go, go, go},
   /*5*/{no, no, no, go, go},
   /*6*/{st, st, go, go, go},
    };
};

TEST_F(HorizontalObstables, NavigatesAroundThem)
{
    sf::Vector2i start = {0, 6};
    sf::Vector2i end   = {0, 0};
    ASSERT_EQ(map.at(start.y).at(start.x), st);
    ASSERT_EQ(map.at(end.y).at(end.x), nd);

    auto path = AStar::run(start, end, dimensions, map);

    EXPECT_TRUE(path.has_path);

    ASSERT_EQ(19, path.path.size());
    EXPECT_EQ(sf::Vector2i(start),  path.path[0]);
    EXPECT_EQ(sf::Vector2i({1, 6}), path.path[1]);
    EXPECT_EQ(sf::Vector2i({2, 6}), path.path[2]);
    EXPECT_EQ(sf::Vector2i({3, 6}), path.path[3]);
    EXPECT_EQ(sf::Vector2i({3, 5}), path.path[4]);
    EXPECT_EQ(sf::Vector2i({3, 4}), path.path[5]);
    EXPECT_EQ(sf::Vector2i({2, 4}), path.path[6]);
    EXPECT_EQ(sf::Vector2i({1, 4}), path.path[7]);
    EXPECT_EQ(sf::Vector2i({0, 4}), path.path[8]);
    EXPECT_EQ(sf::Vector2i({0, 3}), path.path[9]);
    EXPECT_EQ(sf::Vector2i({0, 2}), path.path[10]);
    EXPECT_EQ(sf::Vector2i({1, 2}), path.path[11]);
    EXPECT_EQ(sf::Vector2i({2, 2}), path.path[12]);
    EXPECT_EQ(sf::Vector2i({3, 2}), path.path[13]);
    EXPECT_EQ(sf::Vector2i({3, 1}), path.path[14]);
    EXPECT_EQ(sf::Vector2i({3, 0}), path.path[15]);
    EXPECT_EQ(sf::Vector2i({2, 0}), path.path[16]);
    EXPECT_EQ(sf::Vector2i({1, 0}), path.path[17]);
    EXPECT_EQ(sf::Vector2i(end),    path.path[18]);
}
