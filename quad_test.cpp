#include <gtest/gtest.h>

#include "QuadNode.h"
#include "Point.h"
#include "Rectangle.h"
#include "QuadTree.h"
#include <array>

using namespace utec::spatial;

class QuadTreeTest : public ::testing::Test {
  protected:
    using data_t = int;
    using point_t = Point<data_t, 2>;
    QuadTree<QuadNode<point_t>, Rectangle<point_t>, point_t> tree;
};

TEST_F(QuadTreeTest, emptyPointList) {
  point_t point = {{30, 40}};
  auto node = tree.search(point);
  EXPECT_EQ(node, nullptr);
}

TEST_F(QuadTreeTest, insertOneElement) {
  point_t point = {{30, 40}};
  tree.insert(point);

  auto node = tree.search(point);
  ASSERT_TRUE(node != nullptr);
  EXPECT_EQ(node->get_point(), point);
}

TEST_F(QuadTreeTest, insertSeveralElement) {
  std::vector<point_t> points = {{{30, 40}}, {{5, 25}}, {{10, 12}}, {{70,70}}, {{50, 30}}, {{35, 45}}};

  for(auto& p : points){
    tree.insert(p);
    auto node = tree.search(p);

    ASSERT_TRUE(node != nullptr);
    EXPECT_EQ(node->get_point(), p);
  }
}

TEST_F(QuadTreeTest, simpleRangeTest) {
  std::vector<point_t> points = {{{5, 0}}, {{6, 9}}, {{9, 3}}, {{6, 5}}, {{7, 7}}, {{8, 6}}};

  Rectangle<point_t> region({{8, 2}}, {{10, 4}});

  for(auto& p : points){
    tree.insert(p);
  }

  auto result = tree.range(region);

  ASSERT_EQ(result.size(), 1);
  EXPECT_EQ(result[0], point_t({9, 3}));
}

template <typename T>
T genRandomNumber(T startRange, T endRange)
{
    return startRange + (T)rand()/((T)RAND_MAX/(T)(endRange-startRange));
}

class QuadTreeParamTest : public ::testing::TestWithParam<std::size_t> {
  protected:
    using data_t = int;
    using point_t = Point<data_t, 2>;
    QuadTree<QuadNode<point_t>, Rectangle<point_t>, point_t> tree;
};

TEST_P(QuadTreeParamTest, randomRangeTest) {
  const std::size_t num_points = GetParam();
  const std::size_t min=0, max=1000;

  std::vector<point_t> points;
  for(std::size_t I=0;I<num_points;I++)
    points.push_back(point_t({genRandomNumber<int>(min, max), genRandomNumber<int>(min, max)}));

  for(auto& p : points){
    tree.insert(p);
  }

  Rectangle<point_t> region({{200, 200}}, {{600, 600}});

  auto result = tree.range(region);

  EXPECT_NE(result.size(), 0);
}

INSTANTIATE_TEST_CASE_P(
        GetMaxThreads,
        QuadTreeParamTest,
        ::testing::Values( 10, 100, 1000, 10000));

int main(int argc, char **argv) {
  srand((unsigned)time(0));

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
