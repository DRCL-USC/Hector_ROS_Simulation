function v = OP_ADD()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 18);
  end
  v = vInitialized;
end
