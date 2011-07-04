(*
 * Copyright 2007-2010 Savonet team
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

(**
  * Functions for using LADSPA plugins.
  *
  * @author Samuel Mimram
  *)

(** Version number. *)
val version : unit -> string

(** Major version. *)
val version_major : unit -> int

(** Minor version. *)
val version_minor : unit -> int

(** Plugins. *)
module Plugin :
sig
  (** A plugin. *)
  type t

  (** The file being loaded is not a LADSPA plugin. *)
  exception Not_a_plugin

  (** Load a LADSPA plugin. The argument is the path of a .so file. *)
  val load : string -> t

  (** Unload a LADSPA plugin. *)
  val unload : t -> unit
end

(** Descriptors. *)
module Descriptor :
sig
  (** A descriptor. *)
  type t

  (** Retrieve the [n]-th descriptor of a plugin. *)
  val descriptor : Plugin.t -> int -> t

  (** Retrieve all the descriptors of a plugin. *)
  val descriptors : Plugin.t -> t array

  (** This numeric identifier indicates the plugin type uniquely. Plugin
    * programmers may reserve ranges of IDs from a central body to avoid
    * clashes. Hosts may assume that IDs are below 0x1000000. *)
  val unique_id : t -> int

  (** This identifier can be used as a unique, case-sensitive identifier for the
    * plugin type within the plugin file. Plugin types should be identified by
    * file and label rather than by index or plugin name, which may be changed
    * in new plugin versions. Labels must not contain white-space characters. *)
  val label : t -> string

  (** Name of the plugin (e.g. "Sine Oscillator"). *)
  val name : t -> string

  (** String indicating the maker of the plugin. *)
  val maker : t -> string

  (** String indicating any copyright applying to the plugin. *)
  val copyright : t -> string option

  (** This indicates the number of ports (input AND output) present on the
      plugin. *)
  val port_count : t -> int

  (** Name of the [n]-th port. *)
  val port_name : t -> int -> string

  (** Is the [n]-th port an input? *)
  val port_is_input : t -> int -> bool

  (** Is the [n]-th port an output? *)
  val port_is_output : t -> int -> bool

  (** Is the [n]-th port an audio port? *)
  val port_is_audio : t -> int -> bool

  (** Is the [n]-th port a control port? *)
  val port_is_control : t -> int -> bool

  val port_is_integer : t -> int -> bool

  val port_is_boolean : t -> int -> bool

  val port_is_logarithmic : t -> int -> bool

  (** Get a sensible default value for a control port. *)
  val port_get_default : t -> ?samplerate:int -> int -> float option

  val port_get_min : t -> ?samplerate:int -> int -> float option

  val port_get_max : t -> ?samplerate:int -> int -> float option

  (** Instance of a descriptor. *)
  type instance

  (** [instantiate descr freq len] instantiates the descriptor [descr] with a
    * sampling frequency [freq] indicating that [len] samples should be
    * processed at each run. *)
  val instantiate : t -> int -> int -> instance

  (** Change the number of samples that should be processed at each [run]. *)
  val set_samples : instance -> int -> unit

  (** [connect_audio_port inst p buf ofs] connects the audio port [p] of
    * instance [inst] to the buffer [buf] starting at position [ofs]. *)
  val connect_audio_port : instance -> int -> float array -> int -> unit

  (** Connect an input control port. *)
  val connect_control_port_in : instance -> int -> float -> unit

  (** Connect an output control port. *)
  val connect_control_port_out : instance -> int -> float ref -> unit

  (** Activate (i.e. initialize) a plugin. *)
  val activate : instance -> unit

  (** Deactivate a plugin. *)
  val deactivate : instance -> unit

  (** An input port is not connected. *)
  exception Input_port_not_connected of int

  (** Process samples. *)
  val run : instance -> unit

  (** Internal function used by the DSSI binding. *)
  val pre_run : instance -> unit
  (** Internal function used by the DSSI binding. *)
  val post_run : instance -> unit
  (** Internal function used by the DSSI binding. *)
  val post_run_adding : instance -> unit
end
