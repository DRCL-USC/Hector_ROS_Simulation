function v = OP_LOG()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 24);
  end
  v = vInitialized;
end
