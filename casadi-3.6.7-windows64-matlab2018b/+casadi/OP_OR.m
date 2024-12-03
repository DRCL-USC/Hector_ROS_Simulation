function v = OP_OR()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 42);
  end
  v = vInitialized;
end
