function v = OP_IF_ELSE_ZERO()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 49);
  end
  v = vInitialized;
end
