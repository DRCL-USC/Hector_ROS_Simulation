function v = OPTI_GENERIC_EQUALITY()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 146);
  end
  v = vInitialized;
end
