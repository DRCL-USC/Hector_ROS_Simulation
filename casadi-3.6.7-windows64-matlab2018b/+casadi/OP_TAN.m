function v = OP_TAN()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 32);
  end
  v = vInitialized;
end
