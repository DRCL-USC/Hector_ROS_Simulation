function v = OPTI_DOMAIN_INTEGER()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 157);
  end
  v = vInitialized;
end
