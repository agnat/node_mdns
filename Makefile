BUILDTYPE ?= Release

GCOV_OUT = out/reports/coverage/cpp
NCOV_OUT = out/reports/coverage

TEST_OPTIONS=

ifdef PULSE_BUILD
  TEST_OPTIONS= --ascii --verbose
endif

all:
	$(MAKE) -C out BUILDTYPE=$(BUILDTYPE)

coverage_build:
	$(MAKE) -C out BUILDTYPE=Coverage

test:
	node --expose_gc utils/testrun $(TEST_OPTIONS)

coverage:
	$(MAKE) coverage_run BUILDTYPE=Coverage

coverage_run: coverage_build
	lcov -d out/$(BUILDTYPE)/obj.target/dns_sd/src --zerocounters
	mkdir -p $(GCOV_OUT)/html; 
	NCOV_OUT=$(NCOV_OUT) node --expose_gc utils/testrun $(TEST_OPTIONS)
	lcov --base-directory out \
		 --directory      out/$(BUILDTYPE)/obj.target/dns_sd/src \
		 --output-file    $(GCOV_OUT)/testrun_all.info \
		 --capture
	lcov --output-file    $(GCOV_OUT)/testrun.info \
		 --extract \
		 $(GCOV_OUT)/testrun_all.info "$(abspath .)/*" \
	   | tee $(GCOV_OUT)/lcov.log 
	genhtml --output-directory $(GCOV_OUT)/html \
	        --demangle-cpp \
			$(GCOV_OUT)/testrun.info
	tail -n 3 $(GCOV_OUT)/lcov.log | utils/coverage > $(GCOV_OUT)/coverage.properties

doc:
	utils/docpack

website:
	echo TODO

.PHONY: test citest coverage coverage_build coverage_run doc website 

