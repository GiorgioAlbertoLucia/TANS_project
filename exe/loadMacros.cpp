void loadMacros(TString myopt="fast")
{
    TString opt;
    if(myopt.Contains("force"))   opt = "kfg";
    else                          opt = "kg";

    // BASE CLASSES
    gSystem->CompileMacro("custom_classes/hit/hit.cpp",opt.Data());
    gSystem->CompileMacro("custom_classes/particle/particle.cpp",opt.Data());

    // FILE MANAGMENT
    gSystem->CompileMacro("yaml/Yaml.cpp",opt.Data());
    gSystem->CompileMacro("custom_classes/recorder/recorder.cpp",opt.Data());
    gSystem->CompileMacro("custom_classes/modelBuilder/modelBuilder.cpp", opt.Data());

    // SIMULATION
    gSystem->CompileMacro("custom_classes/event/event.cpp",opt.Data());
    gSystem->CompileMacro("custom_classes/simulation/simulation.cpp",opt.Data());   
    
    // RECONSTRUCTION
    gSystem->CompileMacro("custom_classes/plotter/plotter.cpp",opt.Data());
    gSystem->CompileMacro("custom_classes/reconstruction/reconstruction.cpp",opt.Data());

    // SOFTWARE
    gSystem->CompileMacro("exe/compileAndRunSoftware.cpp",opt.Data());
}
