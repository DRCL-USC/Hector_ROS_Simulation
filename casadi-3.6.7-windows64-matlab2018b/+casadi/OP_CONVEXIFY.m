function v = OP_CONVEXIFY()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 108);
  end
  v = vInitialized;
end
