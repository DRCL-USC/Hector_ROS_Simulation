function v = OP_CEIL()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 44);
  end
  v = vInitialized;
end
