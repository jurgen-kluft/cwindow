package cwindow

import (
	cbase "github.com/jurgen-kluft/cbase/package"
	"github.com/jurgen-kluft/ccode/denv"
)

// GetPackage returns the package object of 'cwindow'
func GetPackage() *denv.Package {
	// Dependencies
	cbasepkg := cbase.GetPackage()

	// The main (cwindow) package
	mainpkg := denv.NewPackage("cwindow")
	mainpkg.AddPackage(cunittestpkg)
	mainpkg.AddPackage(centrypkg)
	mainpkg.AddPackage(cbasepkg)

	// 'cwindow' library
	mainlib := denv.SetupDefaultCppLibProject("cwindow", "github.com\\jurgen-kluft\\cwindow")
	mainlib.Dependencies = append(mainlib.Dependencies, cbasepkg.GetMainLib())

	mainpkg.AddMainLib(mainlib)
	return mainpkg
}
