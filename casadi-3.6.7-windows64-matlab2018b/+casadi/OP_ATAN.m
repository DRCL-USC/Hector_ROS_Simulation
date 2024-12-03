function v = OP_ATAN()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 35);
  end
  v = vInitialized;
end
