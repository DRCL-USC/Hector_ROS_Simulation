function v = OP_MUL()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 20);
  end
  v = vInitialized;
end
