function v = OP_SUB()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 19);
  end
  v = vInitialized;
end
