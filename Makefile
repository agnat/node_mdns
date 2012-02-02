BUILDTYPE ?= Release

GCOV_FLAGS = CXXFLAGS='-g -O0 --coverage' LDFLAGS='-g -O0 --coverage'
all:
	$(MAKE) -C out BUILDTYPE=$(BUILDTYPE)

gcov_build:
	$(MAKE) -C out $(GCOV_FLAGS) BUILDTYPE=$(BUILDTYPE)

test:
	utils/testrun

	cd out; \


coverage: gcov_build
	lcov -d out/$(BUILDTYPE)/obj.target/dns_sd/src --zerocounters
	mkdir -p out/reports/coverage/cpp/html; 
	utils/testrun
	lcov --base-directory out \
		 --directory      out/$(BUILDTYPE)/obj.target/dns_sd/src \
		 --output-file    out/reports/coverage/cpp/testrun_all.info \
		 --test-name mdns_test_suite \
		 --capture
	lcov --output-file    out/reports/coverage/cpp/testrun.info \
		 --extract \
		 out/reports/coverage/cpp/testrun_all.info "*/node_mdns/src/*"
	genhtml --output-directory out/reports/coverage/cpp/html \
			out/reports/coverage/cpp/testrun.info


.PHONY: test coverage gcov_build

