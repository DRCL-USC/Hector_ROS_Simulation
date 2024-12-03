function v = IS_SPARSITY()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 117);
  end
  v = vInitialized;
end
