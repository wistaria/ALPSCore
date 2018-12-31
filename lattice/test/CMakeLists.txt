include(ALPSEnableTests)

set (test_src
  basis
  basis_io
  unitcell
  lattice
  smatrix
  )

foreach(test ${test_src})
    alps_add_gtest(${test})
endforeach(test)
