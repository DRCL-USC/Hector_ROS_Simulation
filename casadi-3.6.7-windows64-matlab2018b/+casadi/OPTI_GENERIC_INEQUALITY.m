function v = OPTI_GENERIC_INEQUALITY()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 147);
  end
  v = vInitialized;
end
