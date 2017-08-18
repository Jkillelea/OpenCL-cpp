CXX = clang++

CFLAGS = -std=c++11
# CFLAGS += -g
# CFLAGS += -O3

LIBS = -lOpenCL
BUILD = obj

all: hello vadd

vadd: $(BUILD)/vadd.o $(BUILD)/util.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LIBS)

hello: $(BUILD)/hello.o $(BUILD)/util.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LIBS)

$(BUILD)/%.o: %.cpp | build_dir
	$(CXX) $(CFLAGS) -o $@ -c $^

build_dir:
	@mkdir -p $(BUILD)

clean:
	rm -r $(BUILD)

# make sure that clean will always be run on `make clean`,
# even if there's a `clean directory or the like`
.PHONY: clean
