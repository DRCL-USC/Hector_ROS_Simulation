function v = OP_BSPLINE()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 107);
  end
  v = vInitialized;
end
