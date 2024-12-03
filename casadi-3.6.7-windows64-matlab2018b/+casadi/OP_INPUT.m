function v = OP_INPUT()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 62);
  end
  v = vInitialized;
end
