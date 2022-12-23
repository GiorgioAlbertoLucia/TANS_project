void loadMacros(TString myopt="fast"){
  TString opt;
  if(myopt.Contains("force")){
    opt = "kfg";
  }
  else {
    opt = "kg";
  }

  // SIMULATION
  gSystem->CompileMacro("pointCC.cpp",opt.Data(), "-", "/custom_classes/pointCC/");
  gSystem->CompileMacro("hit.cpp",opt.Data(), "-", "/custom_classes/hit/");
  gSystem->CompileMacro("particle.cpp",opt.Data(), "-", "/custom_classes/particle/");
  gSystem->CompileMacro("event.cpp",opt.Data(), "-", "/custom_classes/event/");
  gSystem->CompileMacro("simulation.cpp",opt.Data(), "-", "/custom_classes/simulation/");

  // RECONSTRUCTION
}
