function v = OP_MMAX()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 100);
  end
  v = vInitialized;
end
