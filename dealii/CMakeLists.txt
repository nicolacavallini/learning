CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)

SET(DEAL_II_DIR /home/nicola/local/usr/dealii/)

FIND_PACKAGE(deal.II 8.5.0 REQUIRED
  HINTS ${DEAL_II_DIR} ../ ../../ $ENV{DEAL_II_DIR}
  )
DEAL_II_INITIALIZE_CACHED_VARIABLES()

PROJECT(learn_dealii)

ADD_EXECUTABLE(symmetrize_00 symmetrize_00.cc)
DEAL_II_SETUP_TARGET(symmetrize_00)

ADD_EXECUTABLE(symmetrize_01 symmetrize_01.cc)
DEAL_II_SETUP_TARGET(symmetrize_01)

ADD_EXECUTABLE(symmetrize_02 symmetrize_02.cc)
DEAL_II_SETUP_TARGET(symmetrize_02)

ADD_EXECUTABLE(inheritance_00 inheritance_00.cc)
DEAL_II_SETUP_TARGET(inheritance_00)
