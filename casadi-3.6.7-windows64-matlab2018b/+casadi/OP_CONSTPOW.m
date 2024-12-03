function v = OP_CONSTPOW()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 26);
  end
  v = vInitialized;
end
