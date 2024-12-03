function v = OP_EXP()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 23);
  end
  v = vInitialized;
end
