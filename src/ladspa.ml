(*
 * Copyright 2007 Samuel Mimram
 *
 * This file is part of ocaml-ladspa.
 *
 * ocaml-ladspa is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * ocaml-ladspa is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with ocaml-ladspa; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * As a special exception to the GNU Library General Public License, you may
 * link, statically or dynamically, a "work that uses the Library" with a publicly
 * distributed version of the Library to produce an executable file containing
 * portions of the Library, and distribute that executable file under terms of
 * your choice, without any of the additional requirements listed in clause 6
 * of the GNU Library General Public License.
 * By "a publicly distributed version of the Library", we mean either the unmodified
 * Library as distributed by The Savonet Team, or a modified version of the Library that is
 * distributed under the conditions defined in clause 3 of the GNU Library General
 * Public License. This exception does not however invalidate any other reasons why
 * the executable file might be covered by the GNU Library General Public License.
 *
 *)



let () =
  Callback.register_exception "ocaml_ladspa_exn_not_found" Not_found

external version : unit -> string = "ocaml_ladspa_version"

external version_major : unit -> int = "ocaml_ladspa_version_major"

external version_minor : unit -> int = "ocaml_ladspa_version_minor"

module Plugin =
struct
  type t

  exception Not_a_plugin

  external load : string -> t = "ocaml_ladspa_open"

  external unload : t -> unit = "ocaml_ladspa_close"
end

let () =
  Callback.register_exception "ocaml_ladspa_exn_not_a_plugin" Plugin.Not_a_plugin

module Descriptor =
struct
  type t

  external descriptor : Plugin.t -> int -> t = "ocaml_ladspa_descriptor"

  let rec descriptors p n =
    try
      let d = descriptor p n in
      let dd = descriptors p (n+1) in
        d::dd
    with
      | Not_found -> []

  let descriptors p = Array.of_list (descriptors p 0)

  external unique_id : t -> int = "ocaml_ladspa_unique_id"

  external label : t -> string = "ocaml_ladspa_label"

  external name : t -> string = "ocaml_ladspa_name"

  external maker : t -> string = "ocaml_ladspa_maker"

  external copyright : t -> string = "ocaml_ladspa_copyright"

  let copyright d =
    let c = copyright d in
    (* "None" is the standard answer when there is no copyright. *)
    if c = "None" then None else Some c

  external port_count : t -> int = "ocaml_ladspa_port_count"

  external port_names : t -> string array = "ocaml_ladspa_port_names"

  let port_name d n =
    (port_names d).(n)

  external port_is_input : t -> int -> bool = "ocaml_ladspa_port_is_input"

  external port_is_output : t -> int -> bool = "ocaml_ladspa_port_is_output"

  external port_is_control : t -> int -> bool = "ocaml_ladspa_port_is_control"

  external port_is_audio : t -> int -> bool = "ocaml_ladspa_port_is_audio"

  external port_is_integer : t -> int -> bool = "ocaml_ladspa_port_is_integer"

  external port_is_boolean : t -> int -> bool = "ocaml_ladspa_port_is_boolean"

  external port_is_logarithmic : t -> int -> bool = "ocaml_ladspa_port_is_logarithmic"

  external port_get_default : t -> int -> int -> float option = "ocaml_ladspa_port_get_default"

  let port_get_default d ?(samplerate=44100) p = port_get_default d samplerate p

  external port_get_min : t -> int -> int -> float option = "ocaml_ladspa_port_get_min"

  let port_get_min d ?(samplerate=44100) p = port_get_min d samplerate p

  external port_get_max : t -> int -> int -> float option = "ocaml_ladspa_port_get_max"

  let port_get_max d ?(samplerate=44100) p = port_get_max d samplerate p

  type instance

  external get_descriptor : instance -> t = "ocaml_ladspa_get_descriptor"

  external instantiate : t -> int -> int -> instance = "ocaml_ladspa_instantiate"

  external set_samples : instance -> int -> unit = "ocaml_ladspa_set_samples"

  external connect_audio_port : instance -> int -> float array -> int -> unit = "ocaml_ladspa_connect_audio_port"

  external connect_control_port : instance -> int -> float ref -> unit = "ocaml_ladspa_connect_control_port"

  let connect_control_port_in i n v =
    assert (port_is_input (get_descriptor i) n);
    let v = ref v in
      connect_control_port i n v

  let connect_control_port_out i n v =
    assert (port_is_output (get_descriptor i) n);
    connect_control_port i n v

  external activate : instance -> unit = "ocaml_ladspa_activate"

  external deactivate : instance -> unit = "ocaml_ladspa_deactivate"

  exception Input_port_not_connected of int

  external run : instance -> unit = "ocaml_ladspa_run"

  external pre_run : instance -> unit = "ocaml_ladspa_pre_run"
  external post_run : instance -> unit = "ocaml_ladspa_post_run"
  external post_run_adding : instance -> unit = "ocaml_ladspa_post_run_adding"
end

let () =
  Callback.register_exception "ocaml_ladspa_exn_input_port_not_connected" (Descriptor.Input_port_not_connected (-1))
