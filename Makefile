# Word Hunt Solver
#
# Every target runs from this directory. The solver reads its dictionary from
# data/ by relative path, so building or running from anywhere else will not
# find it.

CXX       = g++
CXXFLAGS  = -std=c++11 -Wall -O2 -I.
EMCC      = emcc

BIN       = bin
WEB_OUT  ?= build

# Solver: the Trie build is the default, the map build is kept for comparison
SOLVER_TRIE = back/wordhunt_lib_optimized.cc back/trie.cc
SOLVER_MAP  = back/wordhunt_lib.cc

TERMINAL_UI = front/terminal_ui.cc front/main_terminal.cc

EMFLAGS = \
  -s WASM=1 \
  -s ALLOW_MEMORY_GROWTH=1 \
  -s EXIT_RUNTIME=0 \
  -s EXPORTED_FUNCTIONS='["_solve_board","_free"]' \
  -s EXPORTED_RUNTIME_METHODS='["ccall","UTF8ToString"]' \
  --preload-file data/pairs.txt@data/pairs.txt

all: $(BIN)/wordhunt

# ── Terminal ──────────────────────────────────────────────────────────────
$(BIN)/wordhunt: $(TERMINAL_UI) $(SOLVER_TRIE)
	@mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) -o $@ $(TERMINAL_UI) $(SOLVER_TRIE)

$(BIN)/wordhunt_map: $(TERMINAL_UI) $(SOLVER_MAP)
	@mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) -o $@ $(TERMINAL_UI) $(SOLVER_MAP)

run: $(BIN)/wordhunt
	@./$(BIN)/wordhunt

# The original std::map solver, for comparing against the Trie
run-original: $(BIN)/wordhunt_map
	@./$(BIN)/wordhunt_map

# ── Browser ───────────────────────────────────────────────────────────────
# Set WEB_OUT to build the bundle straight into another project.
web: $(WEB_OUT)/word_hunt.js $(WEB_OUT)/index.html

$(WEB_OUT)/word_hunt.js: back/solver_export.cc $(SOLVER_TRIE)
	@mkdir -p $(WEB_OUT)
	$(EMCC) -std=c++11 -I. -O2 $(EMFLAGS) back/solver_export.cc $(SOLVER_TRIE) -o $@

$(WEB_OUT)/index.html: front/web/index.html
	@mkdir -p $(WEB_OUT)
	cp front/web/index.html $@

# The dictionary is a preloaded data file fetched over HTTP, so the page
# cannot be opened from disk.
run-web: web
	@echo "Serving $(WEB_OUT) at http://localhost:8000"
	@cd $(WEB_OUT) && python3 -m http.server 8000

# ── Tests ─────────────────────────────────────────────────────────────────
$(BIN)/test_trie: test/test_optimized.cc $(SOLVER_TRIE)
	@mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) -o $@ test/test_optimized.cc $(SOLVER_TRIE)

$(BIN)/test_map: test/test_wordhunt.cc $(SOLVER_MAP)
	@mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) -o $@ test/test_wordhunt.cc $(SOLVER_MAP)

test: $(BIN)/test_map $(BIN)/test_trie
	@./$(BIN)/test_map
	@echo
	@./$(BIN)/test_trie

# Builds every target and reports which succeeded
verify:
	@./scripts/test_all_builds.sh

clean:
	rm -rf $(BIN)
	rm -f $(WEB_OUT)/word_hunt.js $(WEB_OUT)/word_hunt.wasm \
	      $(WEB_OUT)/word_hunt.data $(WEB_OUT)/index.html

.PHONY: all run run-original web run-web test verify clean
