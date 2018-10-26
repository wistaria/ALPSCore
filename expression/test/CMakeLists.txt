include(ALPSEnableTests)

set (test_src
  simple
  simple_double
  simple_complex
  params_ex
  params_double
  params_complex
  )

foreach(test ${test_src})
    alps_add_gtest(${test})
endforeach(test)
