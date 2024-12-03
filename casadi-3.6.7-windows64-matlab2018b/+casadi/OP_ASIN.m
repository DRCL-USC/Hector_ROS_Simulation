function v = OP_ASIN()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 33);
  end
  v = vInitialized;
end
