#include <gtest/gtest.h>

#include "QuadNode.h"
#include "Point.h"
#include "Rectangle.h"
#include "QuadTree.h"

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/shared_ptr.hpp>

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

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

namespace boost{ namespace geometry{ namespace index{
template <typename Box>
struct indexable<boost::shared_ptr<Box>>{
  typedef boost::shared_ptr<Box> V;
  typedef Box const& result_type;
  result_type operator()(V const& v) const {return *v;}
};
}}} //boost::geometry::index

typedef bg::model::point<double, 2, bg::cs::cartesian> point;
typedef boost::shared_ptr<point> value;
typedef bg::model::box<point> box;

class QuadTreeParamTest : public ::testing::TestWithParam<std::size_t> {
  protected:
    using data_t = int;
    using point_t = Point<data_t, 2>;
    QuadTree<QuadNode<point_t>, Rectangle<point_t>, point_t> tree;

    bgi::rtree<value, bgi::linear<16, 4>> rtree;
};

TEST_P(QuadTreeParamTest, randomRangeTest) {
  const std::size_t num_points = GetParam();
  const std::size_t min=0, max=1000;

  auto cmp = [](point_t a, point_t b) {
    const int x=0, y=1;
    return (a.get(x) < b.get(x)) || ((a.get(x) == b.get(x)) && (a.get(y) < b.get(y)));
  };

  std::set<point_t, decltype(cmp)> points(cmp);

  for(std::size_t I=0;I<num_points;I++)
    points.insert(point_t({genRandomNumber<int>(min, max), genRandomNumber<int>(min, max)}));

  for(auto& p : points){
    tree.insert(p);

    boost::shared_ptr<point> new_point(new point(p.get(0), p.get(1)));
    rtree.insert(new_point);
  }

  Rectangle<point_t> region({{200, 200}}, {{600, 600}});
  auto result = tree.range(region);

  box query(point(200, 200), point(600, 600));
  std::vector<value> reference_result;
  rtree.query(bgi::intersects(query), std::back_inserter(reference_result));

  EXPECT_EQ(result.size(), reference_result.size());
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
