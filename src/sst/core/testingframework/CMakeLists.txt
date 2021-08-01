install(
  FILES sst_test_engine_loader.py sst-test-core sst-test-elements
  PERMISSIONS
    OWNER_READ
    OWNER_WRITE
    OWNER_EXECUTE
    GROUP_READ
    GROUP_EXECUTE
    WORLD_READ
    WORLD_EXECUTE
  TYPE BIN)

install(
  FILES sst_unittest.py
        sst_unittest_support.py
        sst_unittest_parameterized.py
        test_engine.py
        test_engine_globals.py
        test_engine_support.py
        test_engine_junit.py
        test_engine_unittest.py
  DESTINATION libexec)
