BUILDTYPE ?= Release

GCOV_FLAGS = -g -O0 --coverage
GCOV_OUT   = out/reports/coverage/cpp

all:
	$(MAKE) -C out BUILDTYPE=$(BUILDTYPE)

gcov_build:
	$(MAKE) -C out CXXFLAGS='$(GCOV_FLAGS)' LDFLAGS='$(GCOV_FLAGS)' BUILDTYPE=$(BUILDTYPE)

test:
	utils/testrun

citest:
	utils/testrun --ascii --verbose

coverage: gcov_build
	lcov -d out/$(BUILDTYPE)/obj.target/dns_sd/src --zerocounters
	mkdir -p $(GCOV_OUT)/html; 
	node --expose_gc utils/testrun
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

.PHONY: test citest coverage gcov_build doc website 

