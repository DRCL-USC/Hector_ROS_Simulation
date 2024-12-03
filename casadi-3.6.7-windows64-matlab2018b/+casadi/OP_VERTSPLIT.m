function v = OP_VERTSPLIT()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 81);
  end
  v = vInitialized;
end
