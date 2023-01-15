void loadMacrosRoby(TString myopt="fast")
{
    TString opt;
    if(myopt.Contains("force"))   opt = "kfg";
    else                          opt = "kg";
  
    // SIMULATION
    gSystem->CompileMacro("yaml/Yaml.cpp",opt.Data());
    gSystem->CompileMacro("custom_classes/pointCC/pointCC.cpp",opt.Data());
    gSystem->CompileMacro("custom_classes/hit/hit.cpp",opt.Data());
    gSystem->CompileMacro("custom_classes/particle/particle.cpp",opt.Data());
    gSystem->CompileMacro("custom_classes/event/event.cpp",opt.Data());
    gSystem->CompileMacro("custom_classes/simulation/simulation.cpp",opt.Data());   
    
    // RECONSTRUCTION
    gSystem->CompileMacro("custom_classes/reconstruction/reconstruction.cpp",opt.Data());
    gSystem->CompileMacro("custom_classes/plotter/plotter.cpp",opt.Data());
}
