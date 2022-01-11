(lang dune 2.8)
(version 0.2.2)
(name ladspa)
(source (github savonet/ocaml-ladspa))
(license LGPL-2.1)
(authors "The Savonet Team <savonet-users@lists.sourceforge.net>")
(maintainers "The Savonet Team <savonet-users@lists.sourceforge.net>")

(generate_opam_files true)
(use_standard_c_and_cxx_flags false)

(package
 (name ladspa)
 (synopsis "Bindings for the LADSPA API which provides audio effects")
 (depends
  dune
  dune-configurator
  conf-ladspa)
)
