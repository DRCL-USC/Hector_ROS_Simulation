function v = OPTI_DOMAIN_REAL()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 156);
  end
  v = vInitialized;
end
