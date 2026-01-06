// A generated module for UnisimCpp functions
//
// This module has been generated via dagger init and serves as a reference to
// basic module structure as you get started with Dagger.
//
// Two functions have been pre-created. You can modify, delete, or add to them,
// as needed. They demonstrate usage of arguments and return types using simple
// echo and grep commands. The functions can be called from the dagger CLI or
// from one of the SDKs.
//
// The first line in this comment block is a short description line and the
// rest is a long description with more detail on the module's purpose or usage,
// if appropriate. All modules should have a short description.

package main

import (
	"context"
	"dagger/unisim-cpp/internal/dagger"
	"strings"
)

type UnisimCpp struct{}

// Run core unit tests
func (m *UnisimCpp) Test(
	ctx context.Context,
	// Source code
	// +defaultPath="/"
	// +ignore=["build", "docs"]
	src *dagger.Directory,
) error {
	// create container with source files
	ctr := dag.Container().From("quay.io/sani7/qt-build:latest")

	// retrieve container architecture and provide it as ENVAR inside the container
	{
		platform, err := ctr.Platform(ctx)
		if err != nil {
			return err
		}
		// platform is of the form "linux/arm64"
		// architecture is the second par of the platform string, after the '/'
		architecture := strings.Split(string(platform), "/")[1]
		ctr = ctr.WithEnvVariable("CUBZH_ARCH", architecture)
	}
	_, err := ctr.
		// mount host directory to container and go into it
		WithMountedDirectory("/project", src).
		WithExec([]string{"mkdir", "-p", "/project/build"}).
		WithWorkdir("/project/").
		// execute build commands
		WithExec([]string{"cmake", "-S", ".", "-B", "build", "-G", "Ninja"}).
		// Flags after `--` are transmitted as-is to the build system (Ninja, here)
		// Ninja will stop if this number of errors is reached : NB_MAX_BUILD_ERRORS
		WithExec([]string{"cmake", "--build", "build", "--clean-first", "--target", "tests"}).
		// exec compiled unit tests program
		WithExec([]string{"cmake", "--build", "build", "--target", "test"}).
		Sync(ctx)
	return err
}

// Lint the core source code
func (m *UnisimCpp) Lint(
	ctx context.Context,
	// Source directory
	// +defaultPath="/"
	// +ignore=["*", "!client", "!server", "!tests", "!UI"]
	src *dagger.Directory,
) error {
	_, err := m.Format(ctx, src, true)
	return err
}

// Format the source code using clang tools
func (m *UnisimCpp) Format(
	ctx context.Context,
	// Source code to format
	// +defaultPath="/"
	// +ignore=["*", "!client", "!server", "!tests", "!UI"]
	src *dagger.Directory,
	// Only check, don't fix
	// +optional
	check bool,
) (*dagger.Directory, error) {
	var script string
	if check {
		// set -e: exit on first error
		// set -o pipefail: keep the last non-0 exit code
		// -regex: all .h / .hpp / .c / .cpp files
		// -maxdepth 2: consider the files in /core and /core/tests
		// // --dry-run: do not apply changes
		script = `set -e ; set -o pipefail ; find . -maxdepth 2 -regex '^.*\\.\\(cpp\\|hpp\\|c\\|h\\)$' -print0 | xargs -0 clang-format --dry-run --Werror -style=file`
	} else {
		// -i: apply changes
		// --Werror: consider warnings as errors
		// -style-file: follow the rules from the .clang-format file
		script = `set -e ; set -o pipefail ; find . -maxdepth 2 -regex '^.*\\.\\(cpp\\|hpp\\|c\\|h\\)$' -print0 | xargs -0 clang-format -i --Werror -style=file`
	}
	output := dag.
		Container().
		From("gaetan/clang-tools").
		WithMountedDirectory("/project", src).
		WithWorkdir("/project").
		WithExec([]string{"ash", "-c", script}).
		Directory(".")
	if check {
		return output.Sync(ctx)
	}
	return output, nil
}

// Build the UnisimCpp project
func (m *UnisimCpp) Build(
	ctx context.Context,
	// Source code
	// +defaultPath="/"
	// +ignore=["build", "docs"]
	src *dagger.Directory,
) (*dagger.Directory, error) {
	ctr := dag.Container().From("quay.io/sani7/qt-build:latest")
	output := ctr.
		// mount host directory to container and go into it
		WithMountedDirectory("/project", src).
		WithExec([]string{"mkdir", "-p", "/project/build"}).
		WithWorkdir("/project/").
		// execute build commands
		WithExec([]string{"cmake", "-S", ".", "-B", "build", "-G", "Ninja"}).
		// Flags after `--` are transmitted as-is to the build system (Ninja, here)
		WithExec([]string{"cmake", "--build", "build", "--clean-first", "--target", "all"}).
		Directory(".")
	return output.Sync(ctx)
}