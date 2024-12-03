function v = OP_PARAMETER()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 64);
  end
  v = vInitialized;
end
