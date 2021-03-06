MV  := mv -f
RM  := rm -vf
CP := cp -vf
SED := sed
CC := gcc
CXX := g++
AR := ar

# OPENCV_CFLAGS := -I/opt/opencv/2.2/include/opencv -I/opt/opencv/2.2/include
OPENCV_CFLAGS := -I/home/gunturkun/bin/opencv/2.2.0/include/opencv -I/home/gunturkun/bin/opencv/2.2.0/include
# OPENCV_LIBS := /opt/opencv/2.2/lib/ -lopencv_highgui -lopencv_core -lopencv_imgproc -lopencv_video
OPENCV_LIBS := /home/gunturkun/bin/opencv/2.2.0/lib/ -lopencv_highgui -lopencv_core -lopencv_imgproc -lopencv_video

JSON_LIB := /home/gunturkun/bin/jsoncpp/lib/libjsoncpp.a
KD_TREE_LIB := /home/gunturkun/bin/kdtree/lib/libkdtree.a

CPPFLAGS += -Iinclude $(OPENCV_CFLAGS) -I/home/gunturkun/bin/jsoncpp/include -I/home/gunturkun/bin/kdtree/include
CCFLAGS = -g -Wall --std=c99
# CXXFLAGS = -g -Wall --std=c++11
CXXFLAGS = -g -O3 -Wall --std=c++11
LDFLAGS  += $(OPENCV_LIBS)
ARFLAGS = ruv

# sudo LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libstdc++.so.6 matlab

lib2_sources := \
	src/common/Arguments.cc \
	src/common/Config.cc \
	src/common/Colors.cc \
	src/common/string/StringStream.cc \
	src/common/Random.cc \
	src/common/clustering/Distance.cc \
	src/common/clustering/Cluster.cc \
	src/core/Box.cc \
	src/core/Image.cc \
	src/core/util/ImageBuilder.cc \
	src/core/Warp.cc \
	src/core/IntegralImage.cc \
	src/core/Gaussian.cc \
	src/core/Frame.cc \
	src/core/Sequence.cc \
	src/core/Score.cc \
	src/core/ScoredBox.cc \
	src/tracker/FBPoint.cc \
	src/tracker/Tracker.cc \
	src/tracker/LKTracker.cc \
	src/tracker/StubbedTracker.cc \
	src/detector/core/BoxIterator.cc \
	src/detector/ensemble/EnsembleScore.cc \
	src/detector/ensemble/PixelComparison.cc \
	src/detector/ensemble/CodeGenerator.cc \
	src/detector/ensemble/DecisionTree.cc \
	src/detector/ensemble/Branch.cc \
	src/detector/ensemble/Leaf.cc \
	src/detector/ensemble/BaseClassifier.cc \
	src/detector/ensemble/EnsembleClassifier.cc \
	src/detector/nn/Patch.cc \
	src/detector/nn/ObjectModel.cc \
	src/detector/nn/NNScore.cc \
	src/detector/nn/NearestNeighborClassifier.cc \
	src/detector/Detector.cc \
	src/tld/DetectorResult.cc \
	src/tld/TrackerResult.cc \
	src/tld/TLD.cc \
	src/parttld/VotingSpace.cc \
	src/parttld/PartTLD.cc

lib2_objects := $(patsubst %.cc,%.o,$(lib2_sources))
lib2_target  := libtld.a

test_target := target
test_dir := test
tool2_sources := $(wildcard test/*.cc)
tool2_objects := $(patsubst %.cc,%.o,$(tool2_sources))
tool2_targets := $(patsubst %.cc,$(test_target)/%-app,$(notdir $(tool2_sources)))

all_targets  = $(lib2_target) $(tool2_targets)
all_objects  = $(lib2_objects) $(tool2_objects)
dependencies = $(patsubst %.o,%.d,$(all_objects))

.PHONY: all
all : $(all_targets)

$(lib_target) : $(lib_objects) Makefile
	$(AR) $(ARFLAGS) $@ $(lib_objects)
	@echo ----------------------------------------
	@echo ... Built $@ ...
	@echo ----------------------------------------

$(lib2_target) : $(lib2_objects) Makefile
	$(AR) $(ARFLAGS) $@ $(lib2_objects)
	@echo ----------------------------------------
	@echo ... Built $@ ...
	@echo ----------------------------------------

$(tool2_targets): $(test_target)/%-app: test/%.o $(lib2_target) Makefile
	$(CXX) $(CXXFLAGS) -I$(OPENCV_CFLAGS) $< $(lib2_target) $(JSON_LIB) $(KD_TREE_LIB) $(lib_target) -L$(LDFLAGS) -o $@
	@echo ----------------------------------------
	@echo ... Built $@ ...
	@echo ----------------------------------------

%.o : %.c Makefile
	$(CC) $(CCFLAGS) -c $(CPPFLAGS) $< -o $@

%.o : %.cc Makefile
	$(CXX) $(CXXFLAGS) -c $(CPPFLAGS) $< -o $@

.PHONY: clean_lapack_deps
clean_lapack_deps:
	# make -C ext/CLAPACK-3.2.1/F2CLIBS/libf2c/ clean
	# make -C ext/CLAPACK-3.2.1/BLAS/SRC/ clean
	# make -C ext/CLAPACK-3.2.1/SRC/ clean

.PHONY: clean
clean : clean_lapack_deps
	@$(RM) $(all_targets)
	@find . -iname "*.o" -or -iname "*.d" -or -iname "*~" | xargs $(RM) -
	@echo ----------------------------------------
	@echo ... Clean-up completed ...
	@echo ----------------------------------------

ifneq "$(MAKECMDGOALS)" "clean"
  include $(dependencies)
endif

%.d : %.c
	$(CC) $(CCFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -MM $< | \
	$(SED) 's,\($(notdir $*)\.o\) *:,$(dir $@)\1 $@: ,' > $@.tmp
	$(MV) $@.tmp $@

%.d : %.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -MM $< | \
	$(SED) 's,\($(notdir $*)\.o\) *:,$(dir $@)\1 $@: ,' > $@.tmp
	$(MV) $@.tmp $@
