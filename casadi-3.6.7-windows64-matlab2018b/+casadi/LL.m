function v = LL()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 12);
  end
  v = vInitialized;
end
