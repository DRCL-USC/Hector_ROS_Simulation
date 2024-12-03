function v = OP_TRANSPOSE()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 71);
  end
  v = vInitialized;
end
