function v = OP_INV()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 53);
  end
  v = vInitialized;
end
