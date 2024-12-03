function v = OP_NE()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 39);
  end
  v = vInitialized;
end
