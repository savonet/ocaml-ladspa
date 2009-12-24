/*
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
 * Library as distributed by INRIA, or a modified version of the Library that is
 * distributed under the conditions defined in clause 3 of the GNU Library General
 * Public License. This exception does not however invalidate any other reasons why
 * the executable file might be covered by the GNU Library General Public License.
 *
 */


#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/custom.h>
#include <caml/fail.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <caml/signals.h>

#include <assert.h>
#include <dlfcn.h>
#include <stdio.h>
#include <math.h>

#include <ladspa.h>
#include "ocaml_ladspa.h"

CAMLprim value ocaml_ladspa_version(value unit)
{
  return caml_copy_string(LADSPA_VERSION);
}

CAMLprim value ocaml_ladspa_version_major(value unit)
{
  return Val_int(LADSPA_VERSION_MAJOR);
}

CAMLprim value ocaml_ladspa_version_minor(value unit)
{
  return Val_int(LADSPA_VERSION_MINOR);
}

CAMLprim value ocaml_ladspa_open(value fname)
{
  void *handle = dlopen(String_val(fname), RTLD_LAZY);
  LADSPA_Descriptor_Function ladspa_descriptor;

  if (!handle)
    caml_raise_constant(*caml_named_value("ocaml_ladspa_exn_not_a_plugin"));
  ladspa_descriptor = (LADSPA_Descriptor_Function)dlsym((void*)handle, "ladspa_descriptor");
  if (dlerror() != NULL || !ladspa_descriptor)
  {
    dlclose(handle);
    caml_raise_constant(*caml_named_value("ocaml_ladspa_exn_not_a_plugin"));
  }

  return (value)handle;
}

CAMLprim value ocaml_ladspa_close(value handle)
{
  dlclose((void*)handle);

  return Val_unit;
}

CAMLprim value ocaml_ladspa_descriptor(value handle, value n)
{
  LADSPA_Descriptor_Function ladspa_descriptor = (LADSPA_Descriptor_Function)dlsym((void*)handle, "ladspa_descriptor");
  const LADSPA_Descriptor *d = ladspa_descriptor(Int_val(n));

  if (!d)
    caml_raise_constant(*caml_named_value("ocaml_ladspa_exn_not_found"));

  return Val_LADSPA_descr(d);
}

CAMLprim value ocaml_ladspa_unique_id(value d)
{
  return Val_int(LADSPA_descr_val(d)->UniqueID);
}

CAMLprim value ocaml_ladspa_label(value d)
{
  return caml_copy_string(LADSPA_descr_val(d)->Label);
}

CAMLprim value ocaml_ladspa_name(value d)
{
  return caml_copy_string(LADSPA_descr_val(d)->Name);
}

CAMLprim value ocaml_ladspa_maker(value d)
{
  return caml_copy_string(LADSPA_descr_val(d)->Maker);
}

CAMLprim value ocaml_ladspa_copyright(value d)
{
  return caml_copy_string(LADSPA_descr_val(d)->Copyright);
}

CAMLprim value ocaml_ladspa_port_count(value d)
{
  return Val_int(LADSPA_descr_val(d)->PortCount);
}

CAMLprim value ocaml_ladspa_port_names(value d)
{
  CAMLparam1(d);
  CAMLlocal1(ans);
  const char * const *pn = LADSPA_descr_val(d)->PortNames;
  int i;
  int n = LADSPA_descr_val(d)->PortCount;

  ans = caml_alloc_tuple(n);
  for (i = 0; i < n; i++)
    Store_field(ans, i, caml_copy_string(pn[i]));

  CAMLreturn(ans);
}

CAMLprim value ocaml_ladspa_port_is_input(value d, value n)
{
  return Val_bool(LADSPA_IS_PORT_INPUT(LADSPA_descr_val(d)->PortDescriptors[Int_val(n)]));
}

CAMLprim value ocaml_ladspa_port_is_output(value d, value n)
{
  return Val_bool(LADSPA_IS_PORT_OUTPUT(LADSPA_descr_val(d)->PortDescriptors[Int_val(n)]));
}

CAMLprim value ocaml_ladspa_port_is_control(value d, value n)
{
  return Val_bool(LADSPA_IS_PORT_CONTROL(LADSPA_descr_val(d)->PortDescriptors[Int_val(n)]));
}

CAMLprim value ocaml_ladspa_port_is_audio(value d, value n)
{
  return Val_bool(LADSPA_IS_PORT_AUDIO(LADSPA_descr_val(d)->PortDescriptors[Int_val(n)]));
}

CAMLprim value ocaml_ladspa_port_is_integer(value d, value n)
{
  return Val_bool(LADSPA_IS_HINT_INTEGER(LADSPA_descr_val(d)->PortRangeHints[Int_val(n)].HintDescriptor));
}

CAMLprim value ocaml_ladspa_port_is_boolean(value d, value n)
{
  return Val_bool(LADSPA_IS_HINT_TOGGLED(LADSPA_descr_val(d)->PortRangeHints[Int_val(n)].HintDescriptor));
}

CAMLprim value ocaml_ladspa_port_is_logarithmic(value d, value n)
{
  return Val_bool(LADSPA_IS_HINT_LOGARITHMIC(LADSPA_descr_val(d)->PortRangeHints[Int_val(n)].HintDescriptor));
}

CAMLprim value ocaml_ladspa_port_get_default(value d, value samplerate, value n)
{
  CAMLparam1(d);
  CAMLlocal1(ans);

  assert(LADSPA_IS_PORT_CONTROL(LADSPA_descr_val(d)->PortDescriptors[Int_val(n)]));

  const LADSPA_PortRangeHint ri = LADSPA_descr_val(d)->PortRangeHints[Int_val(n)];
  LADSPA_PortRangeHintDescriptor h = ri.HintDescriptor;
  float lower = ri.LowerBound;
  float upper = ri.UpperBound;
  float def = 0.;

  if (LADSPA_IS_HINT_SAMPLE_RATE(h))
  {
    lower *= Int_val(samplerate);
    upper *= Int_val(samplerate);
  }

  if LADSPA_IS_HINT_HAS_DEFAULT(h)
  {
    if LADSPA_IS_HINT_DEFAULT_MINIMUM(h)
      def = lower;
    else if LADSPA_IS_HINT_DEFAULT_LOW(h)
      if LADSPA_IS_HINT_LOGARITHMIC(h)
        def = exp(log(lower) * 0.75 + log(upper) * 0.25);
      else
        def = lower * 0.75 + upper * 0.25;
    else if LADSPA_IS_HINT_DEFAULT_MIDDLE(h)
      if LADSPA_IS_HINT_LOGARITHMIC(h)
        def = exp(log(lower) * 0.5 + log(upper) * 0.5);
      else
        def = lower * 0.5 + upper * 0.5;
    else if LADSPA_IS_HINT_DEFAULT_HIGH(h)
      if LADSPA_IS_HINT_LOGARITHMIC(h)
        def = exp(log(lower) * 0.25 + log(upper) * 0.75);
      else
        def = lower * 0.25 + upper * 0.75;
    else if LADSPA_IS_HINT_DEFAULT_MAXIMUM(h)
      def = upper;
    else if LADSPA_IS_HINT_DEFAULT_0(h)
      def = 0;
    else if LADSPA_IS_HINT_DEFAULT_1(h)
      def = 1;
    else if LADSPA_IS_HINT_DEFAULT_100(h)
      def = 100;
    else if LADSPA_IS_HINT_DEFAULT_440(h)
      def = 440;

    ans = caml_alloc(1, 0);
    Store_field(ans, 0, caml_copy_double(def));
    CAMLreturn(ans);
  }
  else
    CAMLreturn(Val_int(0));
}

CAMLprim value ocaml_ladspa_port_get_min(value d, value samplerate, value n)
{
  CAMLparam1(d);
  CAMLlocal1(ans);
  const LADSPA_PortRangeHint ri = LADSPA_descr_val(d)->PortRangeHints[Int_val(n)];
  LADSPA_Data bound;

  assert(LADSPA_IS_PORT_CONTROL(LADSPA_descr_val(d)->PortDescriptors[Int_val(n)]));

  if (!LADSPA_IS_HINT_BOUNDED_BELOW(ri.HintDescriptor))
    CAMLreturn(Val_int(0));

  bound = ri.LowerBound;
  if (LADSPA_IS_HINT_SAMPLE_RATE(ri.HintDescriptor))
    bound *= Int_val(samplerate);
  ans = caml_alloc(1, 0);
  Store_field(ans, 0, caml_copy_double(bound));
  CAMLreturn(ans);
}

CAMLprim value ocaml_ladspa_port_get_max(value d, value samplerate, value n)
{
  CAMLparam1(d);
  CAMLlocal1(ans);
  const LADSPA_PortRangeHint ri = LADSPA_descr_val(d)->PortRangeHints[Int_val(n)];
  LADSPA_Data bound;

  assert(LADSPA_IS_PORT_CONTROL(LADSPA_descr_val(d)->PortDescriptors[Int_val(n)]));

  if (!LADSPA_IS_HINT_BOUNDED_ABOVE(ri.HintDescriptor))
    CAMLreturn(Val_int(0));

  bound = ri.UpperBound;
  if (LADSPA_IS_HINT_SAMPLE_RATE(ri.HintDescriptor))
    bound *= Int_val(samplerate);
  ans = caml_alloc(1, 0);
  Store_field(ans, 0, caml_copy_double(bound));
  CAMLreturn(ans);
}

/***** Instances ****/

static void finalize_instance(value inst)
{
  ladspa_instance *instance = Instance_val(inst);
  int i;

  instance->descr->cleanup(instance->handle);
  for (i = 0; i < instance->descr->PortCount; i++)
  {
    caml_remove_global_root(&instance->vbuf[i]);
    free(instance->buf[i]);
  }
  free(instance->vbuf);
  free(instance->buf);
  free(instance->offset);
  free(instance);
}

static struct custom_operations instance_ops =
{
  "ocaml_ladspa_instance",
  finalize_instance,
  custom_compare_default,
  custom_hash_default,
  custom_serialize_default,
  custom_deserialize_default
};


CAMLprim value ocaml_ladspa_instantiate(value d, value rate, value samples)
{
  CAMLparam1(d);
  CAMLlocal1(ans);
  ladspa_instance* instance = malloc(sizeof(ladspa_instance));
  int ports;
  int i;

  instance->descr = LADSPA_descr_val(d);
  ports = instance->descr->PortCount;
  instance->handle = instance->descr->instantiate(instance->descr, Int_val(rate));
  instance->samples = Int_val(samples);
  instance->offset = malloc(ports * sizeof(int));
  instance->buf = malloc(ports * sizeof(LADSPA_Data*));
  instance->vbuf = malloc(ports * sizeof(value));
  /* TODO: alloc less for control ports */
  for (i = 0; i < ports; i++)
  {
    if (LADSPA_IS_PORT_CONTROL(instance->descr->PortDescriptors[i]))
      instance->buf[i] = malloc(sizeof(LADSPA_Data));
    else
      instance->buf[i] = malloc(instance->samples * sizeof(LADSPA_Data));
    instance->vbuf[i] = 0;
    caml_register_global_root(&instance->vbuf[i]);
    instance->descr->connect_port(instance->handle, i, instance->buf[i]);
  }

  ans = caml_alloc_custom(&instance_ops, sizeof(ladspa_instance*), 1, 0);
  Instance_val(ans) = instance;

  CAMLreturn(ans);
}

CAMLprim value ocaml_ladspa_set_samples(value inst, value _samples)
{
  ladspa_instance* instance = Instance_val(inst);
  int ports = instance->descr->PortCount;
  int samples = Int_val(_samples);
  int i;

  if (instance->samples == samples)
    return Val_unit;

  instance->samples = samples;

  if (samples == 0)
    return Val_unit;

  for (i = 0; i < ports; i++)
  {
    if (LADSPA_IS_PORT_AUDIO(instance->descr->PortDescriptors[i]))
    {
      instance->buf[i] = realloc(instance->buf[i], instance->samples * sizeof(LADSPA_Data));
      assert(instance->buf[i]);
      instance->descr->connect_port(instance->handle, i, instance->buf[i]);
    }
  }

  return Val_unit;
}

CAMLprim value ocaml_ladspa_get_descriptor(value i)
{
  ladspa_instance* instance = Instance_val(i);

  return Val_LADSPA_descr(instance->descr);
}

CAMLprim value ocaml_ladspa_connect_audio_port(value i, value _n, value a, value offs)
{
  CAMLparam2(i, a);
  ladspa_instance* instance = Instance_val(i);
  int n = Int_val(_n);

  assert(LADSPA_IS_PORT_AUDIO(instance->descr->PortDescriptors[n]));
  instance->vbuf[n] = a;
  instance->offset[n] = Int_val(offs);

  CAMLreturn(Val_unit);
}

CAMLprim value ocaml_ladspa_connect_control_port(value i, value _n, value a)
{
  CAMLparam2(i, a);
  ladspa_instance* instance = Instance_val(i);
  int n = Int_val(_n);

  assert(LADSPA_IS_PORT_CONTROL(instance->descr->PortDescriptors[n]));
  instance->vbuf[n] = a;

  CAMLreturn(Val_unit);
}

CAMLprim value ocaml_ladspa_activate(value i)
{
  ladspa_instance* instance = Instance_val(i);

  if (instance->descr->activate)
    instance->descr->activate(instance->handle);

  return Val_unit;
}

CAMLprim value ocaml_ladspa_deactivate(value i)
{
  ladspa_instance* instance = Instance_val(i);

  if (instance->descr->deactivate)
    instance->descr->deactivate(instance->handle);

  return Val_unit;
}

CAMLprim value ocaml_ladspa_pre_run(value inst)
{
  ladspa_instance* instance = Instance_val(inst);
  int i, j;

  for (i = 0; i < instance->descr->PortCount; i++)
  {
    if (LADSPA_IS_PORT_INPUT(instance->descr->PortDescriptors[i]))
    {
      if (!instance->vbuf[i])
        caml_raise_with_arg(*caml_named_value("ocaml_ladspa_exn_input_port_not_connected"), Val_int(i));
      if (LADSPA_IS_PORT_CONTROL(instance->descr->PortDescriptors[i]))
        instance->buf[i][0] = Double_val(Field(instance->vbuf[i], 0));
      else
        for (j = 0; j < instance->samples; j++)
          instance->buf[i][j] = Double_field(instance->vbuf[i], j + instance->offset[i]);
    }
  }

  return Val_unit;
}

CAMLprim value ocaml_ladspa_post_run(value inst)
{
  ladspa_instance* instance = Instance_val(inst);
  int i, j;

  for (i = 0; i < instance->descr->PortCount; i++)
  {
    if (LADSPA_IS_PORT_OUTPUT(instance->descr->PortDescriptors[i]) && instance->vbuf[i])
    {
      if (LADSPA_IS_PORT_CONTROL(instance->descr->PortDescriptors[i]))
        Store_field(instance->vbuf[i], 0, caml_copy_double(instance->buf[i][0]));
      else
        for (j = 0; j < instance->samples; j++)
          Store_double_field(instance->vbuf[i], j + instance->offset[i], instance->buf[i][j]);
    }
  }

  return Val_unit;
}

CAMLprim value ocaml_ladspa_post_run_adding(value inst)
{
  ladspa_instance* instance = Instance_val(inst);
  int i, j;

  for (i = 0; i < instance->descr->PortCount; i++)
  {
    if (LADSPA_IS_PORT_OUTPUT(instance->descr->PortDescriptors[i]) && instance->vbuf[i])
    {
      if (LADSPA_IS_PORT_CONTROL(instance->descr->PortDescriptors[i]))
        Store_field(instance->vbuf[i], 0, caml_copy_double(instance->buf[i][0]));
      else
        for (j = 0; j < instance->samples; j++)
          Store_double_field(instance->vbuf[i], j + instance->offset[i], Double_field(instance->vbuf[i],j) + instance->buf[i][j]);
    }
  }

  return Val_unit;
}

CAMLprim value ocaml_ladspa_run(value inst)
{
  CAMLparam1(inst);
  ladspa_instance* instance = Instance_val(inst);

  ocaml_ladspa_pre_run(inst);

  caml_enter_blocking_section();
  instance->descr->run(instance->handle, instance->samples);
  caml_leave_blocking_section();

  ocaml_ladspa_post_run(inst);

  CAMLreturn(Val_unit);
}
