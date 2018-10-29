include(ALPSEnableTests)

set (test_src
  unitcell
  lattice
  )

foreach(test ${test_src})
    alps_add_gtest(${test})
endforeach(test)
