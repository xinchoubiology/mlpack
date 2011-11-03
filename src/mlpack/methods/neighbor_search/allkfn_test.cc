/**
 * @file allkfn_test.cc
 *
 * Test file for AllkFN class
 */
#include <mlpack/core.h>
#include "neighbor_search.h"

#define BOOST_TEST_MODULE AllkFN Test
#include <boost/test/unit_test.hpp>

using namespace mlpack;
using namespace mlpack::neighbor;

/***
 * Simple furthest-neighbors test with small, synthetic dataset.  This is an
 * exhaustive test, which checks that each method for performing the calculation
 * (dual-tree, single-tree, naive) produces the correct results.  An
 * eleven-point dataset and the ten furthest neighbors are taken.  The dataset
 * is in one dimension for simplicity -- the correct functionality of distance
 * functions is not tested here.
 */
BOOST_AUTO_TEST_CASE(exhaustive_synthetic_test) {
  // Set up our data.
  arma::mat data(1, 11);
  data[0] = 0.05; // Row addressing is unnecessary (they are all 0).
  data[1] = 0.35;
  data[2] = 0.15;
  data[3] = 1.25;
  data[4] = 5.05;
  data[5] = -0.22;
  data[6] = -2.00;
  data[7] = -1.30;
  data[8] = 0.45;
  data[9] = 0.90;
  data[10] = 1.00;

  // We will loop through three times, one for each method of performing the
  // calculation.  We'll always use 10 neighbors, so set that parameter.
  CLI::GetParam<int>("neighbor_search/k") = 10;
  for (int i = 0; i < 3; i++) {
    AllkFN* allkfn;
    arma::mat data_mutable = data;
    switch(i) {
      case 0: // Use the dual-tree method.
        allkfn = new AllkFN(data_mutable);
        break;
      case 1: // Use the single-tree method.
        CLI::GetParam<bool>("neighbor_search/single_mode") = true;
        allkfn = new AllkFN(data_mutable);
        break;
      case 2: // Use the naive method.
        CLI::GetParam<bool>("neighbor_search/single_mode") = false;
        CLI::GetParam<bool>("neighbor_search/naive_mode") = true;
        allkfn = new AllkFN(data_mutable);
        break;
    }

    // Now perform the actual calculation.
    arma::Mat<size_t> neighbors;
    arma::mat distances;
    allkfn->ComputeNeighbors(neighbors, distances);

    // Now the exhaustive check for correctness.  This will be long.  We must
    // also remember that the distances returned are squared distances.  As a
    // result, distance comparisons are written out as (distance * distance) for
    // readability.

    // Neighbors of point 0.
    BOOST_REQUIRE(neighbors(9, 0) == 2);
    BOOST_REQUIRE_CLOSE(distances(9, 0), (0.10 * 0.10), 1e-5);
    BOOST_REQUIRE(neighbors(8, 0) == 5);
    BOOST_REQUIRE_CLOSE(distances(8, 0), (0.27 * 0.27), 1e-5);
    BOOST_REQUIRE(neighbors(7, 0) == 1);
    BOOST_REQUIRE_CLOSE(distances(7, 0), (0.30 * 0.30), 1e-5);
    BOOST_REQUIRE(neighbors(6, 0) == 8);
    BOOST_REQUIRE_CLOSE(distances(6, 0), (0.40 * 0.40), 1e-5);
    BOOST_REQUIRE(neighbors(5, 0) == 9);
    BOOST_REQUIRE_CLOSE(distances(5, 0), (0.85 * 0.85), 1e-5);
    BOOST_REQUIRE(neighbors(4, 0) == 10);
    BOOST_REQUIRE_CLOSE(distances(4, 0), (0.95 * 0.95), 1e-5);
    BOOST_REQUIRE(neighbors(3, 0) == 3);
    BOOST_REQUIRE_CLOSE(distances(3, 0), (1.20 * 1.20), 1e-5);
    BOOST_REQUIRE(neighbors(2, 0) == 7);
    BOOST_REQUIRE_CLOSE(distances(2, 0), (1.35 * 1.35), 1e-5);
    BOOST_REQUIRE(neighbors(1, 0) == 6);
    BOOST_REQUIRE_CLOSE(distances(1, 0), (2.05 * 2.05), 1e-5);
    BOOST_REQUIRE(neighbors(0, 0) == 4);
    BOOST_REQUIRE_CLOSE(distances(0, 0), (5.00 * 5.00), 1e-5);

    // Neighbors of point 1.
    BOOST_REQUIRE(neighbors(9, 1) == 8);
    BOOST_REQUIRE_CLOSE(distances(9, 1), (0.10 * 0.10), 1e-5);
    BOOST_REQUIRE(neighbors(8, 1) == 2);
    BOOST_REQUIRE_CLOSE(distances(8, 1), (0.20 * 0.20), 1e-5);
    BOOST_REQUIRE(neighbors(7, 1) == 0);
    BOOST_REQUIRE_CLOSE(distances(7, 1), (0.30 * 0.30), 1e-5);
    BOOST_REQUIRE(neighbors(6, 1) == 9);
    BOOST_REQUIRE_CLOSE(distances(6, 1), (0.55 * 0.55), 1e-5);
    BOOST_REQUIRE(neighbors(5, 1) == 5);
    BOOST_REQUIRE_CLOSE(distances(5, 1), (0.57 * 0.57), 1e-5);
    BOOST_REQUIRE(neighbors(4, 1) == 10);
    BOOST_REQUIRE_CLOSE(distances(4, 1), (0.65 * 0.65), 1e-5);
    BOOST_REQUIRE(neighbors(3, 1) == 3);
    BOOST_REQUIRE_CLOSE(distances(3, 1), (0.90 * 0.90), 1e-5);
    BOOST_REQUIRE(neighbors(2, 1) == 7);
    BOOST_REQUIRE_CLOSE(distances(2, 1), (1.65 * 1.65), 1e-5);
    BOOST_REQUIRE(neighbors(1, 1) == 6);
    BOOST_REQUIRE_CLOSE(distances(1, 1), (2.35 * 2.35), 1e-5);
    BOOST_REQUIRE(neighbors(0, 1) == 4);
    BOOST_REQUIRE_CLOSE(distances(0, 1), (4.70 * 4.70), 1e-5);

    // Neighbors of point 2.
    BOOST_REQUIRE(neighbors(9, 2) == 0);
    BOOST_REQUIRE_CLOSE(distances(9, 2), (0.10 * 0.10), 1e-5);
    BOOST_REQUIRE(neighbors(8, 2) == 1);
    BOOST_REQUIRE_CLOSE(distances(8, 2), (0.20 * 0.20), 1e-5);
    BOOST_REQUIRE(neighbors(7, 2) == 8);
    BOOST_REQUIRE_CLOSE(distances(7, 2), (0.30 * 0.30), 1e-5);
    BOOST_REQUIRE(neighbors(6, 2) == 5);
    BOOST_REQUIRE_CLOSE(distances(6, 2), (0.37 * 0.37), 1e-5);
    BOOST_REQUIRE(neighbors(5, 2) == 9);
    BOOST_REQUIRE_CLOSE(distances(5, 2), (0.75 * 0.75), 1e-5);
    BOOST_REQUIRE(neighbors(4, 2) == 10);
    BOOST_REQUIRE_CLOSE(distances(4, 2), (0.85 * 0.85), 1e-5);
    BOOST_REQUIRE(neighbors(3, 2) == 3);
    BOOST_REQUIRE_CLOSE(distances(3, 2), (1.10 * 1.10), 1e-5);
    BOOST_REQUIRE(neighbors(2, 2) == 7);
    BOOST_REQUIRE_CLOSE(distances(2, 2), (1.45 * 1.45), 1e-5);
    BOOST_REQUIRE(neighbors(1, 2) == 6);
    BOOST_REQUIRE_CLOSE(distances(1, 2), (2.15 * 2.15), 1e-5);
    BOOST_REQUIRE(neighbors(0, 2) == 4);
    BOOST_REQUIRE_CLOSE(distances(0, 2), (4.90 * 4.90), 1e-5);

    // Neighbors of point 3.
    BOOST_REQUIRE(neighbors(9, 3) == 10);
    BOOST_REQUIRE_CLOSE(distances(9, 3), (0.25 * 0.25), 1e-5);
    BOOST_REQUIRE(neighbors(8, 3) == 9);
    BOOST_REQUIRE_CLOSE(distances(8, 3), (0.35 * 0.35), 1e-5);
    BOOST_REQUIRE(neighbors(7, 3) == 8);
    BOOST_REQUIRE_CLOSE(distances(7, 3), (0.80 * 0.80), 1e-5);
    BOOST_REQUIRE(neighbors(6, 3) == 1);
    BOOST_REQUIRE_CLOSE(distances(6, 3), (0.90 * 0.90), 1e-5);
    BOOST_REQUIRE(neighbors(5, 3) == 2);
    BOOST_REQUIRE_CLOSE(distances(5, 3), (1.10 * 1.10), 1e-5);
    BOOST_REQUIRE(neighbors(4, 3) == 0);
    BOOST_REQUIRE_CLOSE(distances(4, 3), (1.20 * 1.20), 1e-5);
    BOOST_REQUIRE(neighbors(3, 3) == 5);
    BOOST_REQUIRE_CLOSE(distances(3, 3), (1.47 * 1.47), 1e-5);
    BOOST_REQUIRE(neighbors(2, 3) == 7);
    BOOST_REQUIRE_CLOSE(distances(2, 3), (2.55 * 2.55), 1e-5);
    BOOST_REQUIRE(neighbors(1, 3) == 6);
    BOOST_REQUIRE_CLOSE(distances(1, 3), (3.25 * 3.25), 1e-5);
    BOOST_REQUIRE(neighbors(0, 3) == 4);
    BOOST_REQUIRE_CLOSE(distances(0, 3), (3.80 * 3.80), 1e-5);

    // Neighbors of point 4.
    BOOST_REQUIRE(neighbors(9, 4) == 3);
    BOOST_REQUIRE_CLOSE(distances(9, 4), (3.80 * 3.80), 1e-5);
    BOOST_REQUIRE(neighbors(8, 4) == 10);
    BOOST_REQUIRE_CLOSE(distances(8, 4), (4.05 * 4.05), 1e-5);
    BOOST_REQUIRE(neighbors(7, 4) == 9);
    BOOST_REQUIRE_CLOSE(distances(7, 4), (4.15 * 4.15), 1e-5);
    BOOST_REQUIRE(neighbors(6, 4) == 8);
    BOOST_REQUIRE_CLOSE(distances(6, 4), (4.60 * 4.60), 1e-5);
    BOOST_REQUIRE(neighbors(5, 4) == 1);
    BOOST_REQUIRE_CLOSE(distances(5, 4), (4.70 * 4.70), 1e-5);
    BOOST_REQUIRE(neighbors(4, 4) == 2);
    BOOST_REQUIRE_CLOSE(distances(4, 4), (4.90 * 4.90), 1e-5);
    BOOST_REQUIRE(neighbors(3, 4) == 0);
    BOOST_REQUIRE_CLOSE(distances(3, 4), (5.00 * 5.00), 1e-5);
    BOOST_REQUIRE(neighbors(2, 4) == 5);
    BOOST_REQUIRE_CLOSE(distances(2, 4), (5.27 * 5.27), 1e-5);
    BOOST_REQUIRE(neighbors(1, 4) == 7);
    BOOST_REQUIRE_CLOSE(distances(1, 4), (6.35 * 6.35), 1e-5);
    BOOST_REQUIRE(neighbors(0, 4) == 6);
    BOOST_REQUIRE_CLOSE(distances(0, 4), (7.05 * 7.05), 1e-5);

    // Neighbors of point 5.
    BOOST_REQUIRE(neighbors(9, 5) == 0);
    BOOST_REQUIRE_CLOSE(distances(9, 5), (0.27 * 0.27), 1e-5);
    BOOST_REQUIRE(neighbors(8, 5) == 2);
    BOOST_REQUIRE_CLOSE(distances(8, 5), (0.37 * 0.37), 1e-5);
    BOOST_REQUIRE(neighbors(7, 5) == 1);
    BOOST_REQUIRE_CLOSE(distances(7, 5), (0.57 * 0.57), 1e-5);
    BOOST_REQUIRE(neighbors(6, 5) == 8);
    BOOST_REQUIRE_CLOSE(distances(6, 5), (0.67 * 0.67), 1e-5);
    BOOST_REQUIRE(neighbors(5, 5) == 7);
    BOOST_REQUIRE_CLOSE(distances(5, 5), (1.08 * 1.08), 1e-5);
    BOOST_REQUIRE(neighbors(4, 5) == 9);
    BOOST_REQUIRE_CLOSE(distances(4, 5), (1.12 * 1.12), 1e-5);
    BOOST_REQUIRE(neighbors(3, 5) == 10);
    BOOST_REQUIRE_CLOSE(distances(3, 5), (1.22 * 1.22), 1e-5);
    BOOST_REQUIRE(neighbors(2, 5) == 3);
    BOOST_REQUIRE_CLOSE(distances(2, 5), (1.47 * 1.47), 1e-5);
    BOOST_REQUIRE(neighbors(1, 5) == 6);
    BOOST_REQUIRE_CLOSE(distances(1, 5), (1.78 * 1.78), 1e-5);
    BOOST_REQUIRE(neighbors(0, 5) == 4);
    BOOST_REQUIRE_CLOSE(distances(0, 5), (5.27 * 5.27), 1e-5);

    // Neighbors of point 6.
    BOOST_REQUIRE(neighbors(9, 6) == 7);
    BOOST_REQUIRE_CLOSE(distances(9, 6), (0.70 * 0.70), 1e-5);
    BOOST_REQUIRE(neighbors(8, 6) == 5);
    BOOST_REQUIRE_CLOSE(distances(8, 6), (1.78 * 1.78), 1e-5);
    BOOST_REQUIRE(neighbors(7, 6) == 0);
    BOOST_REQUIRE_CLOSE(distances(7, 6), (2.05 * 2.05), 1e-5);
    BOOST_REQUIRE(neighbors(6, 6) == 2);
    BOOST_REQUIRE_CLOSE(distances(6, 6), (2.15 * 2.15), 1e-5);
    BOOST_REQUIRE(neighbors(5, 6) == 1);
    BOOST_REQUIRE_CLOSE(distances(5, 6), (2.35 * 2.35), 1e-5);
    BOOST_REQUIRE(neighbors(4, 6) == 8);
    BOOST_REQUIRE_CLOSE(distances(4, 6), (2.45 * 2.45), 1e-5);
    BOOST_REQUIRE(neighbors(3, 6) == 9);
    BOOST_REQUIRE_CLOSE(distances(3, 6), (2.90 * 2.90), 1e-5);
    BOOST_REQUIRE(neighbors(2, 6) == 10);
    BOOST_REQUIRE_CLOSE(distances(2, 6), (3.00 * 3.00), 1e-5);
    BOOST_REQUIRE(neighbors(1, 6) == 3);
    BOOST_REQUIRE_CLOSE(distances(1, 6), (3.25 * 3.25), 1e-5);
    BOOST_REQUIRE(neighbors(0, 6) == 4);
    BOOST_REQUIRE_CLOSE(distances(0, 6), (7.05 * 7.05), 1e-5);

    // Neighbors of point 7.
    BOOST_REQUIRE(neighbors(9, 7) == 6);
    BOOST_REQUIRE_CLOSE(distances(9, 7), (0.70 * 0.70), 1e-5);
    BOOST_REQUIRE(neighbors(8, 7) == 5);
    BOOST_REQUIRE_CLOSE(distances(8, 7), (1.08 * 1.08), 1e-5);
    BOOST_REQUIRE(neighbors(7, 7) == 0);
    BOOST_REQUIRE_CLOSE(distances(7, 7), (1.35 * 1.35), 1e-5);
    BOOST_REQUIRE(neighbors(6, 7) == 2);
    BOOST_REQUIRE_CLOSE(distances(6, 7), (1.45 * 1.45), 1e-5);
    BOOST_REQUIRE(neighbors(5, 7) == 1);
    BOOST_REQUIRE_CLOSE(distances(5, 7), (1.65 * 1.65), 1e-5);
    BOOST_REQUIRE(neighbors(4, 7) == 8);
    BOOST_REQUIRE_CLOSE(distances(4, 7), (1.75 * 1.75), 1e-5);
    BOOST_REQUIRE(neighbors(3, 7) == 9);
    BOOST_REQUIRE_CLOSE(distances(3, 7), (2.20 * 2.20), 1e-5);
    BOOST_REQUIRE(neighbors(2, 7) == 10);
    BOOST_REQUIRE_CLOSE(distances(2, 7), (2.30 * 2.30), 1e-5);
    BOOST_REQUIRE(neighbors(1, 7) == 3);
    BOOST_REQUIRE_CLOSE(distances(1, 7), (2.55 * 2.55), 1e-5);
    BOOST_REQUIRE(neighbors(0, 7) == 4);
    BOOST_REQUIRE_CLOSE(distances(0, 7), (6.35 * 6.35), 1e-5);

    // Neighbors of point 8.
    BOOST_REQUIRE(neighbors(9, 8) == 1);
    BOOST_REQUIRE_CLOSE(distances(9, 8), (0.10 * 0.10), 1e-5);
    BOOST_REQUIRE(neighbors(8, 8) == 2);
    BOOST_REQUIRE_CLOSE(distances(8, 8), (0.30 * 0.30), 1e-5);
    BOOST_REQUIRE(neighbors(7, 8) == 0);
    BOOST_REQUIRE_CLOSE(distances(7, 8), (0.40 * 0.40), 1e-5);
    BOOST_REQUIRE(neighbors(6, 8) == 9);
    BOOST_REQUIRE_CLOSE(distances(6, 8), (0.45 * 0.45), 1e-5);
    BOOST_REQUIRE(neighbors(5, 8) == 10);
    BOOST_REQUIRE_CLOSE(distances(5, 8), (0.55 * 0.55), 1e-5);
    BOOST_REQUIRE(neighbors(4, 8) == 5);
    BOOST_REQUIRE_CLOSE(distances(4, 8), (0.67 * 0.67), 1e-5);
    BOOST_REQUIRE(neighbors(3, 8) == 3);
    BOOST_REQUIRE_CLOSE(distances(3, 8), (0.80 * 0.80), 1e-5);
    BOOST_REQUIRE(neighbors(2, 8) == 7);
    BOOST_REQUIRE_CLOSE(distances(2, 8), (1.75 * 1.75), 1e-5);
    BOOST_REQUIRE(neighbors(1, 8) == 6);
    BOOST_REQUIRE_CLOSE(distances(1, 8), (2.45 * 2.45), 1e-5);
    BOOST_REQUIRE(neighbors(0, 8) == 4);
    BOOST_REQUIRE_CLOSE(distances(0, 8), (4.60 * 4.60), 1e-5);

    // Neighbors of point 9.
    BOOST_REQUIRE(neighbors(9, 9) == 10);
    BOOST_REQUIRE_CLOSE(distances(9, 9), (0.10 * 0.10), 1e-5);
    BOOST_REQUIRE(neighbors(8, 9) == 3);
    BOOST_REQUIRE_CLOSE(distances(8, 9), (0.35 * 0.35), 1e-5);
    BOOST_REQUIRE(neighbors(7, 9) == 8);
    BOOST_REQUIRE_CLOSE(distances(7, 9), (0.45 * 0.45), 1e-5);
    BOOST_REQUIRE(neighbors(6, 9) == 1);
    BOOST_REQUIRE_CLOSE(distances(6, 9), (0.55 * 0.55), 1e-5);
    BOOST_REQUIRE(neighbors(5, 9) == 2);
    BOOST_REQUIRE_CLOSE(distances(5, 9), (0.75 * 0.75), 1e-5);
    BOOST_REQUIRE(neighbors(4, 9) == 0);
    BOOST_REQUIRE_CLOSE(distances(4, 9), (0.85 * 0.85), 1e-5);
    BOOST_REQUIRE(neighbors(3, 9) == 5);
    BOOST_REQUIRE_CLOSE(distances(3, 9), (1.12 * 1.12), 1e-5);
    BOOST_REQUIRE(neighbors(2, 9) == 7);
    BOOST_REQUIRE_CLOSE(distances(2, 9), (2.20 * 2.20), 1e-5);
    BOOST_REQUIRE(neighbors(1, 9) == 6);
    BOOST_REQUIRE_CLOSE(distances(1, 9), (2.90 * 2.90), 1e-5);
    BOOST_REQUIRE(neighbors(0, 9) == 4);
    BOOST_REQUIRE_CLOSE(distances(0, 9), (4.15 * 4.15), 1e-5);

    // Neighbors of point 10.
    BOOST_REQUIRE(neighbors(9, 10) == 9);
    BOOST_REQUIRE_CLOSE(distances(9, 10), (0.10 * 0.10), 1e-5);
    BOOST_REQUIRE(neighbors(8, 10) == 3);
    BOOST_REQUIRE_CLOSE(distances(8, 10), (0.25 * 0.25), 1e-5);
    BOOST_REQUIRE(neighbors(7, 10) == 8);
    BOOST_REQUIRE_CLOSE(distances(7, 10), (0.55 * 0.55), 1e-5);
    BOOST_REQUIRE(neighbors(6, 10) == 1);
    BOOST_REQUIRE_CLOSE(distances(6, 10), (0.65 * 0.65), 1e-5);
    BOOST_REQUIRE(neighbors(5, 10) == 2);
    BOOST_REQUIRE_CLOSE(distances(5, 10), (0.85 * 0.85), 1e-5);
    BOOST_REQUIRE(neighbors(4, 10) == 0);
    BOOST_REQUIRE_CLOSE(distances(4, 10), (0.95 * 0.95), 1e-5);
    BOOST_REQUIRE(neighbors(3, 10) == 5);
    BOOST_REQUIRE_CLOSE(distances(3, 10), (1.22 * 1.22), 1e-5);
    BOOST_REQUIRE(neighbors(2, 10) == 7);
    BOOST_REQUIRE_CLOSE(distances(2, 10), (2.30 * 2.30), 1e-5);
    BOOST_REQUIRE(neighbors(1, 10) == 6);
    BOOST_REQUIRE_CLOSE(distances(1, 10), (3.00 * 3.00), 1e-5);
    BOOST_REQUIRE(neighbors(0, 10) == 4);
    BOOST_REQUIRE_CLOSE(distances(0, 10), (4.05 * 4.05), 1e-5);

    // Clean the memory.
    delete allkfn;
  }
}

/***
 * Test the dual-tree furthest-neighbors method with the naive method.  This
 * uses both a query and reference dataset.
 *
 * Errors are produced if the results are not identical.
 */
BOOST_AUTO_TEST_CASE(dual_tree_vs_naive_1) {
  arma::mat data_for_tree_;

  // Hard-coded filename: bad!
  if (!data_for_tree_.load("test_data_3_1000.csv", arma::auto_detect, false,
      true))
    BOOST_FAIL("Cannot load test dataset test_data_3_1000.csv!");

  // Set up matrices to work with.
  arma::mat dual_query(data_for_tree_);
  arma::mat dual_references(data_for_tree_);
  arma::mat naive_query(data_for_tree_);
  arma::mat naive_references(data_for_tree_);

  CLI::GetParam<bool>("neighbor_search/naive_mode") = false;
  CLI::GetParam<bool>("neighbor_search/single_mode") = false;
  AllkFN allkfn_(dual_query, dual_references);

  CLI::GetParam<bool>("neighbor_search/naive_mode") = true;
  AllkFN naive_(naive_query, naive_references);

  arma::Mat<size_t> resulting_neighbors_tree;
  arma::mat distances_tree;
  allkfn_.ComputeNeighbors(resulting_neighbors_tree, distances_tree);

  arma::Mat<size_t> resulting_neighbors_naive;
  arma::mat distances_naive;
  naive_.ComputeNeighbors(resulting_neighbors_naive, distances_naive);

  for (size_t i = 0; i < resulting_neighbors_tree.n_elem; i++) {
    BOOST_REQUIRE(resulting_neighbors_tree[i] == resulting_neighbors_naive[i]);
    BOOST_REQUIRE_CLOSE(distances_tree[i], distances_naive[i], 1e-5);
  }
}

/**
 * Test the dual-tree furthest-neighbors method with the naive method.  This
 * uses only a reference dataset.
 *
 * Errors are produced if the results are not identical.
 */
BOOST_AUTO_TEST_CASE(dual_tree_vs_naive_2) {
  arma::mat data_for_tree_;

  // Hard-coded filename: bad!
  // Code duplication: also bad!
  if (!data_for_tree_.load("test_data_3_1000.csv", arma::auto_detect, false,
      true))
    BOOST_FAIL("Cannot load test dataset test_data_3_1000.csv!");

  // Set up matrices to work with (may not be necessary with no ALIAS_MATRIX?).
  arma::mat dual_references(data_for_tree_);
  arma::mat naive_references(data_for_tree_);

  CLI::GetParam<bool>("neighbor_search/naive_mode") = false;
  CLI::GetParam<bool>("neighbor_search/single_mode") = false;
  AllkFN allkfn_(dual_references);

  CLI::GetParam<bool>("neighbor_search/naive_mode") = true;
  AllkFN naive_(naive_references);

  arma::Mat<size_t> resulting_neighbors_tree;
  arma::mat distances_tree;
  allkfn_.ComputeNeighbors(resulting_neighbors_tree, distances_tree);

  arma::Mat<size_t> resulting_neighbors_naive;
  arma::mat distances_naive;
  naive_.ComputeNeighbors(resulting_neighbors_naive, distances_naive);

  for (size_t i = 0; i < resulting_neighbors_tree.n_elem; i++) {
    BOOST_REQUIRE(resulting_neighbors_tree[i] == resulting_neighbors_naive[i]);
    BOOST_REQUIRE_CLOSE(distances_tree[i], distances_naive[i], 1e-5);
  }
}

/**
 * Test the single-tree furthest-neighbors method with the naive method.  This
 * uses only a reference dataset.
 *
 * Errors are produced if the results are not identical.
 */
BOOST_AUTO_TEST_CASE(single_tree_vs_naive) {
  arma::mat data_for_tree_;

  // Hard-coded filename: bad!
  // Code duplication: also bad!
  if (!data_for_tree_.load("test_data_3_1000.csv", arma::auto_detect, false,
      true))
    BOOST_FAIL("Cannot load test dataset test_data_3_1000.csv!");

  arma::mat single_query(data_for_tree_);
  arma::mat naive_query(data_for_tree_);

  CLI::GetParam<bool>("neighbor_search/naive_mode") = false;
  CLI::GetParam<bool>("neighbor_search/single_mode") = true;
  AllkFN allkfn_(single_query);

  CLI::GetParam<bool>("neighbor_search/naive_mode") = true;
  CLI::GetParam<bool>("neighbor_search/single_mode") = false;
  AllkFN naive_(naive_query);

  arma::Mat<size_t> resulting_neighbors_tree;
  arma::mat distances_tree;
  allkfn_.ComputeNeighbors(resulting_neighbors_tree, distances_tree);

  arma::Mat<size_t> resulting_neighbors_naive;
  arma::mat distances_naive;
  naive_.ComputeNeighbors(resulting_neighbors_naive, distances_naive);

  for (size_t i = 0; i < resulting_neighbors_tree.n_elem; i++) {
    BOOST_REQUIRE(resulting_neighbors_tree[i] == resulting_neighbors_naive[i]);
    BOOST_REQUIRE_CLOSE(distances_tree[i], distances_naive[i], 1e-5);
  }
}