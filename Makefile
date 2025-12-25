PROJ = rbx

# Allow override (e.g., `make VERSION=1.2.3`)
VERSION ?= $(shell (git describe --tags 2>/dev/null || echo "develop") | sed 's/^v//')
REVISION ?= $(shell git rev-parse --short HEAD)

BUILD_DIR = build-rbx-min

.PHONY: help setup config build install release test clean all

all: setup config build test

##@ Dependencies
setup: ## Clone all components
	@git submodule update --init --recursive

config: ## Configure
	./scripts/config-rbx.sh $(BUILD_DIR)

##@ Development
build: ## Build all components
	cmake --build $(BUILD_DIR)

##@ Testing
test: ## Run the tests
	./scripts/test-rbx.sh $(BUILD_DIR)

##@ Maintenance
clean: ## Remove all build artifacts
	rm -rf $(BUILD_DIR)

help: ## Display this help
	@awk 'BEGIN {FS = ":.*##"; printf "Usage:\n  make \033[36m<target>\033[0m\n"} /^[.a-zA-Z_-]+:.*?##/ { printf "  \033[36m%-15s\033[0m %s\n", $$1, $$2 } /^##@/ { printf "\n\033[1m%s\033[0m\n", substr($$0, 5) } ' $(MAKEFILE_LIST)
