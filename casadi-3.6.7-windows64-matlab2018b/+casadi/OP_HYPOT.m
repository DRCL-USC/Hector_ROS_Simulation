function v = OP_HYPOT()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 112);
  end
  v = vInitialized;
end
