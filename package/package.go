package cwindow

import (
	cbase "github.com/jurgen-kluft/cbase/package"
	"github.com/jurgen-kluft/ccode/denv"
	cunittest "github.com/jurgen-kluft/cunittest/package"
)

// GetPackage returns the package object of 'cwindow'
func GetPackage() *denv.Package {
	// Dependencies
	basepkg := cbase.GetPackage()
	unittestpkg := cunittest.GetPackage()

	// The main (cwindow) package
	mainpkg := denv.NewPackage("cwindow")
	mainpkg.AddPackage(unittestpkg)
	mainpkg.AddPackage(basepkg)

	// 'cwindow' library
	mainlib := denv.SetupCppLibProject("cwindow", "github.com\\jurgen-kluft\\cwindow")
	mainlib.AddDependencies(basepkg.GetMainLib()...)

	// unittest project
	maintest := denv.SetupDefaultCppTestProject("cwindow_test", "github.com\\jurgen-kluft\\cwindow")
	maintest.AddDependencies(unittestpkg.GetMainLib()...)
	maintest.Dependencies = append(maintest.Dependencies, mainlib)

	mainpkg.AddMainLib(mainlib)
	mainpkg.AddUnittest(maintest)
	return mainpkg
}
