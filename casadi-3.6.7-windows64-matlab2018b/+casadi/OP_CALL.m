function v = OP_CALL()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 65);
  end
  v = vInitialized;
end
