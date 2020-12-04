module C = Configurator.V1

let () =
  C.main ~name:"ladspa-header" (fun c ->
      try
        let defined =
          C.C_define.import c ~includes:["ladspa.h"]
            [("LADSPA_VERSION", C.C_define.Type.String)]
        in
        match defined with
          | [("LADSPA_VERSION", C.C_define.Value.String _)] ->
              C.C_define.gen_header_file c ~fname:"config.h"
                [("HAS_LADSPA", C.C_define.Value.String "true")]
          | _ -> assert false
      with _ ->
        failwith
          "Cannot find ladspa.h, please install the LADSPA_SDK, or put \
           ladspa.h somewhere it can be found")
