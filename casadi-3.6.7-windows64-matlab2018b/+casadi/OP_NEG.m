function v = OP_NEG()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 22);
  end
  v = vInitialized;
end
