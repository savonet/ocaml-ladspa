typedef struct
{
  LADSPA_Descriptor *descr;
  LADSPA_Handle handle;
  LADSPA_Data **buf;
  int *offset;
  value *vbuf;
  int samples;
} ladspa_instance;

#define Instance_val(v) (*((ladspa_instance**)Data_custom_val(v)))
