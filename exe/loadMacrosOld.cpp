void loadMacrosOld()
{

  // SIMULATION
  gROOT->LoadMacro("yaml/Yaml.cpp+");
  gROOT->LoadMacro("custom_classes/pointCC/pointCC.cpp+");
  gROOT->LoadMacro("custom_classes/hit/hit.cpp+");
  gROOT->LoadMacro("custom_classes/particle/particle.cpp+");
  gROOT->LoadMacro("custom_classes/event/event.cpp+");
  gROOT->LoadMacro("custom_classes/simulation/simulation.cpp+");

  // RECONSTRUCTION
}
